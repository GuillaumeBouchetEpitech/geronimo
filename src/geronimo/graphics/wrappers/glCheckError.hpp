
namespace gero {
namespace graphics {

namespace debug {

void glCheckError(const int8_t* file, uint32_t line, const int8_t* expression);

} // namespace debug

#if 0

#define glCheck(expr)                                                          \
  {                                                                            \
    expr;                                                                      \
    debug::glCheckError(__FILE__, __LINE__, #expr);                            \
  }

#else

#define glCheck(expr) (expr)

#endif

} // namespace graphics
} // namespace gero
