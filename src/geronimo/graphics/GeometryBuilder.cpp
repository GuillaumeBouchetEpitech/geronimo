
#include "GeometryBuilder.hpp"

#include "ShaderProgram.hpp"

namespace gero {
namespace graphics {

GeometryBuilder::GeometryBuilder() { reset(); }

//

GeometryBuilder& GeometryBuilder::reset() {
  _shader = nullptr;
  _def.primitiveType = Geometry::PrimitiveType::triangles;
  _def.vbos.clear();

  return *this;
}

GeometryBuilder& GeometryBuilder::setShader(IUnboundShaderProgram& shader) {
  _shader = &shader;

  return *this;
}

GeometryBuilder& GeometryBuilder::setPrimitiveType(Geometry::PrimitiveType primitiveType) {
  _def.primitiveType = primitiveType;

  return *this;
}

GeometryBuilder& GeometryBuilder::addVbo() {
  _def.vbos.push_back(Geometry::Definition::Vbo());

  return *this;
}

GeometryBuilder& GeometryBuilder::setVboAsInstanced() {
  if (_def.vbos.empty())
    D_THROW(std::runtime_error, "cannot add attrs, no vbo defined");

  _def.vbos.back().instanced = true;

  return *this;
}

GeometryBuilder& GeometryBuilder::setVboAsDynamic() {
  if (_def.vbos.empty())
    D_THROW(std::runtime_error, "cannot add attrs, no vbo defined");

  _def.vbos.back().dynamic = true;

  return *this;
}

GeometryBuilder& GeometryBuilder::addVboAttribute(const std::string& name,
                                                  Geometry::AttrType type,
                                                  int32_t index,
                                                  bool isIgnored /*= false*/) {
  if (_def.vbos.empty())
    D_THROW(std::runtime_error, "cannot add attrs, no vbo defined");

  _def.vbos.back().attrs.emplace_back(name, type, index, isIgnored);

  return *this;
}

GeometryBuilder& GeometryBuilder::addVboAttribute(const std::string& name, Geometry::AttrType type) {
  return addVboAttribute(name, type, -1, false);
}

GeometryBuilder& GeometryBuilder::addIgnoredVboAttribute(const std::string& name, Geometry::AttrType type) {
  return addVboAttribute(name, type, -1, true);
}

GeometryBuilder&
GeometryBuilder::addIgnoredVboAttribute(const std::string& name, Geometry::AttrType type, int32_t index) {
  return addVboAttribute(name, type, index, true);
}

GeometryBuilder& GeometryBuilder::setVboStride(uint32_t stride) {
  if (_def.vbos.empty())
    D_THROW(std::runtime_error, "cannot add attrs, no vbo defined");

  _def.vbos.back().stride = int32_t(stride);

  return *this;
}

GeometryBuilder& GeometryBuilder::validate() {
  if (!_shader)
    D_THROW(std::runtime_error, "cannot build, no shader");

  if (_def.vbos.empty())
    D_THROW(std::runtime_error, "cannot build, no vbo(s) defined");

  for (const auto& vbo : _def.vbos) {
    if (vbo.attrs.empty())
      D_THROW(std::runtime_error, "cannot build, no vbo.attrs defined");

    for (const auto& attr : vbo.attrs)
      if (!attr.ignored && !_shader->hasAttribute(attr.name.c_str()))
        D_THROW(std::runtime_error, "cannot build, vbo.attr not found, name: \"" << attr.name << "\"");
  }

  return *this;
}

Geometry& GeometryBuilder::build(Geometry& geometry) {
  validate();
  geometry.initialize(*_shader, _def);
  return geometry;
}

Geometry::Definition GeometryBuilder::getDefinition(bool inDoValidate /*= true*/) {
  if (inDoValidate)
    validate();
  return _def;
}

} // namespace graphics
} // namespace gero
