
#pragma once

#include "Geometry.hpp"

namespace gero {
namespace graphics {

class IUnboundShaderProgram;

class GeometryBuilder {
private:
  Geometry::Definition _def;
  IUnboundShaderProgram* _shader = nullptr;

public:
  GeometryBuilder();
  ~GeometryBuilder() = default;

public:
  GeometryBuilder& reset();

  GeometryBuilder& setShader(IUnboundShaderProgram& shader);
  GeometryBuilder& setPrimitiveType(Geometry::PrimitiveType primitiveType);
  GeometryBuilder& addVbo();
  GeometryBuilder& setVboAsInstanced();
  GeometryBuilder& setVboAsDynamic();
  GeometryBuilder&
  addVboAttribute(const std::string& name, Geometry::AttrType type, int32_t index, bool isIgnored = false);
  GeometryBuilder& addVboAttribute(const std::string& name, Geometry::AttrType type);
  GeometryBuilder& addIgnoredVboAttribute(const std::string& name, Geometry::AttrType type);
  GeometryBuilder& addIgnoredVboAttribute(const std::string& name, Geometry::AttrType type, int32_t index);
  GeometryBuilder& setVboStride(uint32_t stride);

  GeometryBuilder& validate();

  Geometry& build(Geometry& geometry);
  Geometry::Definition getDefinition(bool inDoValidate = true);
};

} // namespace graphics
} // namespace gero
