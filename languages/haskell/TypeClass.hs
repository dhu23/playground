newtype MyNewType a = MyNewType { run :: Maybe a }

class ExTypeClass g where
  bar :: String -> g ()

instance ExTypeClass MyNewType where
  bar s = MyNewType $ Just ()

-- foo :: ExTypeClass g => g ()
-- without the type annotation, it gives
-- ambiguous type variable 'g0' arising from a use of 'bar'
-- prevents the constraint '(ExTypeClass g0)' from being solved
-- foo = bar "\n"
