-- CREATE(keyword)
-- CONFIGURE(keyword) { configuration }
-- example:

CREATE(let)
CONFIGURE(let) {
  -- First, set the keyword as something the compiler can detect. For this, we will assign it as a variable wrapper.
  set let as VarWrapper;
}