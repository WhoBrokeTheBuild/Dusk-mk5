#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <dmf_generated.h>
#include <zlib.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using namespace flatbuffers;
using namespace dmf;

string GetBasename(const string & path)
{
  size_t pos = path.find_last_of("/\\");
  if (string::npos != pos)
  {
    return path.substr(pos);
  }
  return path;
}

string GetDirname(const string & path)
{
  size_t pos = path.find_last_of("/\\");
  if (string::npos != pos)
  {
    return path.substr(0, pos);
  }
  return string();
}

bool load_obj(const string & filename, FlatBufferBuilder & fbb)
{
  bool                        ret;
  string                      err;
  tinyobj::attrib_t           attrib;
  vector<tinyobj::shape_t>    shapes;
  vector<tinyobj::material_t> materials;

  ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(),
                         GetDirname(filename).c_str());

  if (!ret)
  {
    fprintf(stderr, "Failed to load %s: %s\n", filename.c_str(), err.c_str());
    return false;
  }

  bool has_norms = !attrib.normals.empty();
  bool has_txcds = !attrib.texcoords.empty();

  vector<flatbuffers::Offset<Shape>> fb_shapes;
  for (tinyobj::shape_t & shape : shapes)
  {
    printf("Shape: %s\n", shape.name.c_str());
    tinyobj::mesh_t &     mesh = shape.mesh;
    tinyobj::material_t * mat  = NULL;

    if (!mesh.material_ids.empty() && mesh.material_ids[0] >= 0)
    {
      mat = &materials[mesh.material_ids[0]];
    }

    auto name = fbb.CreateString(shape.name);

    vector<float> verts;
    vector<float> norms;
    vector<float> txcds;

    verts.resize(mesh.indices.size() * 3);

    if (has_norms)
    {
      norms.resize(mesh.indices.size() * 3);
    }

    if (has_txcds)
    {
      txcds.resize(mesh.indices.size() * 2);
    }

    size_t index = 0;
    for (size_t f = 0; f < mesh.num_face_vertices.size(); ++f)
    {
      size_t fv = mesh.num_face_vertices[f];

      for (size_t v = 0; v < fv; ++v)
      {
        tinyobj::index_t idx = mesh.indices[index + v];

        verts[(index + v) * 3 + 0] = attrib.vertices[3 * idx.vertex_index + 0];
        verts[(index + v) * 3 + 1] = attrib.vertices[3 * idx.vertex_index + 1];
        verts[(index + v) * 3 + 2] = attrib.vertices[3 * idx.vertex_index + 2];

        if (has_norms)
        {
          norms[(index + v) * 3 + 0] = attrib.normals[3 * idx.normal_index + 0];
          norms[(index + v) * 3 + 1] = attrib.normals[3 * idx.normal_index + 1];
          norms[(index + v) * 3 + 2] = attrib.normals[3 * idx.normal_index + 2];
        }

        if (has_txcds)
        {
          txcds[(index + v) * 2 + 0] =
              attrib.texcoords[2 * idx.texcoord_index + 0];
          txcds[(index + v) * 2 + 1] =
              attrib.texcoords[2 * idx.texcoord_index + 1];
        }
      }
      index += fv;
    }

    auto vert_vec =
        fbb.CreateVectorOfStructs((Vec3 *)verts.data(), verts.size() / 3);
    auto norm_vec =
        fbb.CreateVectorOfStructs((Vec3 *)norms.data(), norms.size() / 3);
    auto txcd_vec =
        fbb.CreateVectorOfStructs((Vec2 *)txcds.data(), txcds.size() / 2);

    Offset<Material> dmf_mat;
    if (NULL != mat)
    {
      auto mat_name = fbb.CreateString(mat->name);
      auto ambient  = Color(mat->ambient[0], mat->ambient[1], mat->ambient[2]);
      auto diffuse  = Color(mat->diffuse[0], mat->diffuse[1], mat->diffuse[2]);
      auto specular =
          Color(mat->specular[0], mat->specular[1], mat->specular[2]);
      auto ambient_map  = fbb.CreateString(mat->ambient_texname);
      auto diffuse_map  = fbb.CreateString(mat->diffuse_texname);
      auto specular_map = fbb.CreateString(mat->specular_texname);
      auto bump_map     = fbb.CreateString(mat->bump_texname);

      MaterialBuilder mb(fbb);
      mb.add_name(mat_name);
      mb.add_ambient(&ambient);
      mb.add_diffuse(&diffuse);
      mb.add_specular(&specular);
      mb.add_ambient_map(ambient_map);
      mb.add_diffuse_map(diffuse_map);
      mb.add_specular_map(specular_map);
      mb.add_bump_map(bump_map);
      dmf_mat = mb.Finish();
    }

    ShapeBuilder sb(fbb);
    sb.add_name(name);
    sb.add_vertices(vert_vec);
    sb.add_normals(norm_vec);
    sb.add_texcoords(txcd_vec);
    sb.add_material(dmf_mat);
    fb_shapes.push_back(sb.Finish());
  }

  auto model = CreateModel(fbb, fbb.CreateVector(fb_shapes));
  fbb.Finish(model);

  return true;
}

bool save_dmf(const string &        old_filename,
              const unsigned char * buffer,
              const size_t &        size)
{
  const size_t CHUNK_SIZE  = 131072; // 128K
  const int    FILEID_SIZE = 4;

  size_t     dot;
  ofstream   file;
  string     filename, zfilename;
  uint64_t   old_size = size;
  const char fileid[] = "DMF-", zfileid[] = "DMFZ";

  int           zerr;
  z_stream      zs;
  unsigned int  have;
  unsigned char out[CHUNK_SIZE];

  filename = old_filename;
  dot      = filename.find_last_of('.');
  if (string::npos != dot)
  {
    filename  = old_filename.substr(0, dot);
    zfilename = old_filename.substr(0, dot);
  }
  filename += ".dmf";
  zfilename += ".dmfz";

  file.open(filename, std::ios::binary);
  if (!file)
  {
    fprintf(stderr, "Failed to open %s\n", filename.c_str());
    goto error;
  }

  file.write(fileid, FILEID_SIZE);
  file.write((char *)buffer, size);
  file.close();

  printf("Wrote %s\n", filename.c_str());

  file.open(zfilename, std::ios::binary);
  if (!file)
  {
    fprintf(stderr, "Failed to open %s\n", zfilename.c_str());
    goto error;
  }

  file.write(zfileid, FILEID_SIZE);
  file.write((const char *)&old_size, sizeof(old_size));

  memset(&zs, 0, sizeof(zs));
  zs.zalloc = Z_NULL;
  zs.zfree  = Z_NULL;
  zs.opaque = Z_NULL;
  zerr      = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
  if (Z_OK != zerr)
  {
    fprintf(stderr, "Failed to initialize zlib: %d\n", zerr);
    goto error;
  }

  zs.avail_in = size;
  zs.next_in  = (unsigned char *)buffer;

  do
  {
    zs.avail_out = CHUNK_SIZE;
    zs.next_out  = out;

    zerr = deflate(&zs, Z_FINISH);
    if (Z_STREAM_ERROR == zerr)
    {
      fprintf(stderr, "Failed to compress file: %d\n", zerr);
      goto error;
    }

    have = CHUNK_SIZE - zs.avail_out;
    if (!file.write((const char *)out, have))
    {
      fprintf(stderr, "Failed to write to file\n");
      goto error;
    }
  } while (zs.avail_out == 0);

  deflateEnd(&zs);

  file.close();

  printf("Wrote %s\n", zfilename.c_str());

  return true;

error:
  file.close();
  deflateEnd(&zs);

  return false;
}

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s [OPTIONS] FILE\n", argv[0]);
    return 0;
  }

  FlatBufferBuilder fbb;
  string            filename = string(argv[1]);

  if (!load_obj(filename, fbb))
  {
    return 1;
  }

  if (!save_dmf(filename, fbb.GetBufferPointer(), fbb.GetSize()))
  {
    return 1;
  }

  return 0;
}
