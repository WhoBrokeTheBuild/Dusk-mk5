#include "dusk/mesh.h"

#include "debug.h"
#include <dusk/resource.h>

void _dusk_mesh_update_shader_data(dusk_mesh_t * this);

void dusk_mesh_init(dusk_mesh_t * this,
                    dusk_material_t * material,
                    dusk_shader_t *   shader,
                    unsigned int      count,
                    const float *     verts,
                    const float *     norms,
                    const float *     txcds)
{
    this->count      = count;
    this->_material  = material;
    this->_shader    = shader;
    this->_draw_mode = GL_TRIANGLES;

    dusk_bind_resource(this->_material);
    dusk_bind_resource(this->_shader);

    glGenVertexArrays(1, &this->_vao);
    glBindVertexArray(this->_vao);

    GLuint vbos[3];
    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_VERTS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_VERTS);

    if (norms)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, norms, GL_STATIC_DRAW);
        glVertexAttribPointer(DUSK_ATTRID_NORMS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DUSK_ATTRID_NORMS);
    }

    if (txcds)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 2, txcds, GL_STATIC_DRAW);
        glVertexAttribPointer(DUSK_ATTRID_TXCDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DUSK_ATTRID_TXCDS);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _dusk_mesh_update_shader_data(this);
    this->_shader_data_id = dusk_shader_add_data(this->_shader, "MeshData", &this->_shader_data,
                                                 sizeof(dusk_mesh_data_t));
    DEBUG_INFO("MeshData ID %d", this->_shader_data_id);
}

void dusk_mesh_term(dusk_mesh_t * this)
{
    glDeleteVertexArrays(1, &this->_vao);
    dusk_free_resource(this->_material);
    dusk_free_resource(this->_shader);
}

void dusk_mesh_render(dusk_mesh_t * this)
{
    if (this->_shader_data_invalid)
    {
        _dusk_mesh_update_shader_data(this);
    }

    dusk_shader_bind(this->_shader);
    dusk_shader_set_data(this->_shader, this->_shader_data_id, &this->_shader_data);

    if (NULL != this->_material)
    {
        dusk_material_bind(this->_material);
    }

    glBindVertexArray(this->_vao);
    glDrawArrays(this->_draw_mode, 0, this->count);
    glBindVertexArray(0);
}

void dusk_mesh_create_plane(dusk_mesh_t * this,
                            dusk_material_t * material,
                            dusk_shader_t *   shader,
                            int               rows,
                            int               cols,
                            float             width,
                            float             height)
{
    this->_material  = material;
    this->_shader    = shader;
    this->_draw_mode = GL_TRIANGLE_STRIP;
    this->count      = (rows * cols) + (rows - 1) * (cols - 2);

    float square_width  = width / (float)cols;
    float square_height = height / (float)rows;

    vec3f_t * tmpverts = malloc(sizeof(vec3f_t) * (rows * cols));
    vec3f_t * tmpnorms = malloc(sizeof(vec3f_t) * (rows * cols));
    vec2f_t * tmptxcds = malloc(sizeof(vec2f_t) * (rows * cols));

    vec3f_t * verts = malloc(sizeof(vec3f_t) * this->count);
    vec3f_t * norms = malloc(sizeof(vec3f_t) * this->count);
    vec2f_t * txcds = malloc(sizeof(vec3f_t) * this->count);

    int i = 0;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            tmpverts[i] = (vec3f_t){{(float)col * square_width, 0.0f, (float)row * square_height}};
            tmpnorms[i] = (vec3f_t){{0.0f, 1.0f, 0.0f}};
            tmptxcds[i] = (vec2f_t){{(float)col / (float)cols, (float)row / (float)rows}};
            ++i;
        }
    }

    i = 0;
    for (int row = 0; row < rows - 1; row++)
    {
        if ((row & 1) == 0)
        { // even rows
            for (int col = 0; col < cols; col++)
            {
                verts[i] = tmpverts[col + row * cols];
                norms[i] = tmpnorms[col + row * cols];
                txcds[i] = tmptxcds[col + row * cols];
                ++i;
                verts[i] = tmpverts[col + (row + 1) * cols];
                norms[i] = tmpnorms[col + (row + 1) * cols];
                txcds[i] = tmptxcds[col + (row + 1) * cols];
                ++i;
            }
        }
        else
        { // odd rows
            for (int col = cols - 1; col > 0; col--)
            {
                verts[i] = tmpverts[col + (row + 1) * cols];
                norms[i] = tmpnorms[col + (row + 1) * cols];
                txcds[i] = tmptxcds[col + (row + 1) * cols];
                ++i;
                verts[i] = tmpverts[col - 1 + row * cols];
                norms[i] = tmpnorms[col - 1 + row * cols];
                txcds[i] = tmptxcds[col - 1 + row * cols];
                ++i;
            }
        }
    }
    if ((rows & 1) && rows > 2)
    {
        verts[i] = tmpverts[(rows - 1) * cols];
        norms[i] = tmpnorms[(rows - 1) * cols];
        txcds[i] = tmptxcds[(rows - 1) * cols];
        ++i;
    }

    free(tmpverts);
    free(tmpnorms);
    free(tmptxcds);

    glGenVertexArrays(1, &this->_vao);
    glBindVertexArray(this->_vao);

    GLuint vbos[3];
    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_VERTS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_VERTS);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, norms, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_NORMS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_NORMS);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 2, txcds, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_TXCDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_TXCDS);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(verts);
    free(norms);
    free(txcds);

    _dusk_mesh_update_shader_data(this);
    this->_shader_data_id = dusk_shader_add_data(this->_shader, "MeshData", &this->_shader_data,
                                                 sizeof(dusk_mesh_data_t));
    DEBUG_INFO("MeshData ID %d", this->_shader_data_id);
}

void _dusk_mesh_update_shader_data(dusk_mesh_t * this)
{
    dusk_mesh_data_t * data = &this->_shader_data;

    if (NULL == this->_material)
    {
        vec4f_copy(&data->ambient, &(vec4f_t){{0.2, 0.2, 0.2, 1.0}});
        vec4f_copy(&data->diffuse, &(vec4f_t){{0.2, 0.2, 0.2, 1.0}});
        vec4f_copy(&data->specular, &(vec4f_t){{0.0, 0.0, 0.0, 1.0}});

        data->shininess = 32;
        data->dissolve  = 0;

        data->has_ambient_map  = false;
        data->has_diffuse_map  = false;
        data->has_specular_map = false;
        data->has_bump_map     = false;
    }
    else
    {
        dusk_material_t * mat = this->_material;

        vec4f_copy(&data->ambient, &mat->ambient);
        vec4f_copy(&data->diffuse, &mat->diffuse);
        vec4f_copy(&data->specular, &mat->specular);

        data->shininess = mat->shininess;
        data->dissolve  = mat->dissolve;

        data->has_ambient_map  = dusk_material_has_ambient_map(mat);
        data->has_diffuse_map  = dusk_material_has_diffuse_map(mat);
        data->has_specular_map = dusk_material_has_specular_map(mat);
        data->has_bump_map     = dusk_material_has_bump_map(mat);
    }

    this->_shader_data_invalid = false;
}
