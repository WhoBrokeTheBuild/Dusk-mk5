// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_DMF_DMF_H_
#define FLATBUFFERS_GENERATED_DMF_DMF_H_

#include "flatbuffers/flatbuffers.h"

namespace dmf {

struct Vec2;

struct Vec3;

struct Color;

struct Material;

struct Shape;

struct Model;

MANUALLY_ALIGNED_STRUCT(4) Vec2 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;

 public:
  Vec2() {
    memset(this, 0, sizeof(Vec2));
  }
  Vec2(const Vec2 &_o) {
    memcpy(this, &_o, sizeof(Vec2));
  }
  Vec2(float _x, float _y)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
};
STRUCT_END(Vec2, 8);

MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(this, 0, sizeof(Vec3));
  }
  Vec3(const Vec3 &_o) {
    memcpy(this, &_o, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
STRUCT_END(Vec3, 12);

MANUALLY_ALIGNED_STRUCT(4) Color FLATBUFFERS_FINAL_CLASS {
 private:
  float r_;
  float g_;
  float b_;

 public:
  Color() {
    memset(this, 0, sizeof(Color));
  }
  Color(const Color &_o) {
    memcpy(this, &_o, sizeof(Color));
  }
  Color(float _r, float _g, float _b)
      : r_(flatbuffers::EndianScalar(_r)),
        g_(flatbuffers::EndianScalar(_g)),
        b_(flatbuffers::EndianScalar(_b)) {
  }
  float r() const {
    return flatbuffers::EndianScalar(r_);
  }
  float g() const {
    return flatbuffers::EndianScalar(g_);
  }
  float b() const {
    return flatbuffers::EndianScalar(b_);
  }
};
STRUCT_END(Color, 12);

struct Material FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_AMBIENT = 6,
    VT_DIFFUSE = 8,
    VT_SPECULAR = 10,
    VT_SHININESS = 12,
    VT_DISSOLVE = 14,
    VT_AMBIENT_MAP = 16,
    VT_DIFFUSE_MAP = 18,
    VT_SPECULAR_MAP = 20,
    VT_BUMP_MAP = 22
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const Color *ambient() const {
    return GetStruct<const Color *>(VT_AMBIENT);
  }
  const Color *diffuse() const {
    return GetStruct<const Color *>(VT_DIFFUSE);
  }
  const Color *specular() const {
    return GetStruct<const Color *>(VT_SPECULAR);
  }
  float shininess() const {
    return GetField<float>(VT_SHININESS, 0.0f);
  }
  float dissolve() const {
    return GetField<float>(VT_DISSOLVE, 0.0f);
  }
  const flatbuffers::String *ambient_map() const {
    return GetPointer<const flatbuffers::String *>(VT_AMBIENT_MAP);
  }
  const flatbuffers::String *diffuse_map() const {
    return GetPointer<const flatbuffers::String *>(VT_DIFFUSE_MAP);
  }
  const flatbuffers::String *specular_map() const {
    return GetPointer<const flatbuffers::String *>(VT_SPECULAR_MAP);
  }
  const flatbuffers::String *bump_map() const {
    return GetPointer<const flatbuffers::String *>(VT_BUMP_MAP);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<Color>(verifier, VT_AMBIENT) &&
           VerifyField<Color>(verifier, VT_DIFFUSE) &&
           VerifyField<Color>(verifier, VT_SPECULAR) &&
           VerifyField<float>(verifier, VT_SHININESS) &&
           VerifyField<float>(verifier, VT_DISSOLVE) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_AMBIENT_MAP) &&
           verifier.Verify(ambient_map()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DIFFUSE_MAP) &&
           verifier.Verify(diffuse_map()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SPECULAR_MAP) &&
           verifier.Verify(specular_map()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_BUMP_MAP) &&
           verifier.Verify(bump_map()) &&
           verifier.EndTable();
  }
};

struct MaterialBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Material::VT_NAME, name);
  }
  void add_ambient(const Color *ambient) {
    fbb_.AddStruct(Material::VT_AMBIENT, ambient);
  }
  void add_diffuse(const Color *diffuse) {
    fbb_.AddStruct(Material::VT_DIFFUSE, diffuse);
  }
  void add_specular(const Color *specular) {
    fbb_.AddStruct(Material::VT_SPECULAR, specular);
  }
  void add_shininess(float shininess) {
    fbb_.AddElement<float>(Material::VT_SHININESS, shininess, 0.0f);
  }
  void add_dissolve(float dissolve) {
    fbb_.AddElement<float>(Material::VT_DISSOLVE, dissolve, 0.0f);
  }
  void add_ambient_map(flatbuffers::Offset<flatbuffers::String> ambient_map) {
    fbb_.AddOffset(Material::VT_AMBIENT_MAP, ambient_map);
  }
  void add_diffuse_map(flatbuffers::Offset<flatbuffers::String> diffuse_map) {
    fbb_.AddOffset(Material::VT_DIFFUSE_MAP, diffuse_map);
  }
  void add_specular_map(flatbuffers::Offset<flatbuffers::String> specular_map) {
    fbb_.AddOffset(Material::VT_SPECULAR_MAP, specular_map);
  }
  void add_bump_map(flatbuffers::Offset<flatbuffers::String> bump_map) {
    fbb_.AddOffset(Material::VT_BUMP_MAP, bump_map);
  }
  MaterialBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MaterialBuilder &operator=(const MaterialBuilder &);
  flatbuffers::Offset<Material> Finish() {
    const auto end = fbb_.EndTable(start_, 10);
    auto o = flatbuffers::Offset<Material>(end);
    return o;
  }
};

inline flatbuffers::Offset<Material> CreateMaterial(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    const Color *ambient = 0,
    const Color *diffuse = 0,
    const Color *specular = 0,
    float shininess = 0.0f,
    float dissolve = 0.0f,
    flatbuffers::Offset<flatbuffers::String> ambient_map = 0,
    flatbuffers::Offset<flatbuffers::String> diffuse_map = 0,
    flatbuffers::Offset<flatbuffers::String> specular_map = 0,
    flatbuffers::Offset<flatbuffers::String> bump_map = 0) {
  MaterialBuilder builder_(_fbb);
  builder_.add_bump_map(bump_map);
  builder_.add_specular_map(specular_map);
  builder_.add_diffuse_map(diffuse_map);
  builder_.add_ambient_map(ambient_map);
  builder_.add_dissolve(dissolve);
  builder_.add_shininess(shininess);
  builder_.add_specular(specular);
  builder_.add_diffuse(diffuse);
  builder_.add_ambient(ambient);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Material> CreateMaterialDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const Color *ambient = 0,
    const Color *diffuse = 0,
    const Color *specular = 0,
    float shininess = 0.0f,
    float dissolve = 0.0f,
    const char *ambient_map = nullptr,
    const char *diffuse_map = nullptr,
    const char *specular_map = nullptr,
    const char *bump_map = nullptr) {
  return dmf::CreateMaterial(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      ambient,
      diffuse,
      specular,
      shininess,
      dissolve,
      ambient_map ? _fbb.CreateString(ambient_map) : 0,
      diffuse_map ? _fbb.CreateString(diffuse_map) : 0,
      specular_map ? _fbb.CreateString(specular_map) : 0,
      bump_map ? _fbb.CreateString(bump_map) : 0);
}

struct Shape FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_MATERIAL = 6,
    VT_VERTICES = 8,
    VT_NORMALS = 10,
    VT_TEXCOORDS = 12
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const Material *material() const {
    return GetPointer<const Material *>(VT_MATERIAL);
  }
  const flatbuffers::Vector<const Vec3 *> *vertices() const {
    return GetPointer<const flatbuffers::Vector<const Vec3 *> *>(VT_VERTICES);
  }
  const flatbuffers::Vector<const Vec3 *> *normals() const {
    return GetPointer<const flatbuffers::Vector<const Vec3 *> *>(VT_NORMALS);
  }
  const flatbuffers::Vector<const Vec2 *> *texcoords() const {
    return GetPointer<const flatbuffers::Vector<const Vec2 *> *>(VT_TEXCOORDS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MATERIAL) &&
           verifier.VerifyTable(material()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_VERTICES) &&
           verifier.Verify(vertices()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_NORMALS) &&
           verifier.Verify(normals()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXCOORDS) &&
           verifier.Verify(texcoords()) &&
           verifier.EndTable();
  }
};

struct ShapeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Shape::VT_NAME, name);
  }
  void add_material(flatbuffers::Offset<Material> material) {
    fbb_.AddOffset(Shape::VT_MATERIAL, material);
  }
  void add_vertices(flatbuffers::Offset<flatbuffers::Vector<const Vec3 *>> vertices) {
    fbb_.AddOffset(Shape::VT_VERTICES, vertices);
  }
  void add_normals(flatbuffers::Offset<flatbuffers::Vector<const Vec3 *>> normals) {
    fbb_.AddOffset(Shape::VT_NORMALS, normals);
  }
  void add_texcoords(flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> texcoords) {
    fbb_.AddOffset(Shape::VT_TEXCOORDS, texcoords);
  }
  ShapeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShapeBuilder &operator=(const ShapeBuilder &);
  flatbuffers::Offset<Shape> Finish() {
    const auto end = fbb_.EndTable(start_, 5);
    auto o = flatbuffers::Offset<Shape>(end);
    return o;
  }
};

inline flatbuffers::Offset<Shape> CreateShape(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<Material> material = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Vec3 *>> vertices = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Vec3 *>> normals = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> texcoords = 0) {
  ShapeBuilder builder_(_fbb);
  builder_.add_texcoords(texcoords);
  builder_.add_normals(normals);
  builder_.add_vertices(vertices);
  builder_.add_material(material);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Shape> CreateShapeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    flatbuffers::Offset<Material> material = 0,
    const std::vector<const Vec3 *> *vertices = nullptr,
    const std::vector<const Vec3 *> *normals = nullptr,
    const std::vector<const Vec2 *> *texcoords = nullptr) {
  return dmf::CreateShape(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      material,
      vertices ? _fbb.CreateVector<const Vec3 *>(*vertices) : 0,
      normals ? _fbb.CreateVector<const Vec3 *>(*normals) : 0,
      texcoords ? _fbb.CreateVector<const Vec2 *>(*texcoords) : 0);
}

struct Model FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_SHAPES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Shape>> *shapes() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Shape>> *>(VT_SHAPES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SHAPES) &&
           verifier.Verify(shapes()) &&
           verifier.VerifyVectorOfTables(shapes()) &&
           verifier.EndTable();
  }
};

struct ModelBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_shapes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shape>>> shapes) {
    fbb_.AddOffset(Model::VT_SHAPES, shapes);
  }
  ModelBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ModelBuilder &operator=(const ModelBuilder &);
  flatbuffers::Offset<Model> Finish() {
    const auto end = fbb_.EndTable(start_, 1);
    auto o = flatbuffers::Offset<Model>(end);
    return o;
  }
};

inline flatbuffers::Offset<Model> CreateModel(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shape>>> shapes = 0) {
  ModelBuilder builder_(_fbb);
  builder_.add_shapes(shapes);
  return builder_.Finish();
}

inline flatbuffers::Offset<Model> CreateModelDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Shape>> *shapes = nullptr) {
  return dmf::CreateModel(
      _fbb,
      shapes ? _fbb.CreateVector<flatbuffers::Offset<Shape>>(*shapes) : 0);
}

inline const dmf::Model *GetModel(const void *buf) {
  return flatbuffers::GetRoot<dmf::Model>(buf);
}

inline bool VerifyModelBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<dmf::Model>(nullptr);
}

inline void FinishModelBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<dmf::Model> root) {
  fbb.Finish(root);
}

}  // namespace dmf

#endif  // FLATBUFFERS_GENERATED_DMF_DMF_H_
