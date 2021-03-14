type Person = String

father :: Person -> Maybe Person
father = undefined


mother :: Person -> Maybe Person
mother = undefined


maternalGrandfather :: Person -> Maybe Person
maternalGrandfather p = mother p >>= father


paternalGrandfather :: Person -> Maybe Person
paternalGrandfather p = father p >>= mother

-- Applicative
bothGrandfathers :: Person -> Maybe (Person, Person)
bothGrandfathers p = (,) <$> paternalGrandfather p <*> maternalGrandfather p

-- Monad
bothGrandfathers' :: Person -> Maybe (Person, Person)
bothGrandfathers' p = do
    gf1 <- paternalGrandfather p
    gf2 <- maternalGrandfather p
    return (gf1, gf2)

-- m >>= return               = m
-- return x >>= f             = f x
-- (m >>= f) >>= g            = m >>= (\x -> f x >>= g)

bothGrandfathers' p = 
    father p >>= 
        (\dad -> father dad >>=
            (\gf1 -> mother p >>= 
                (\mom -> father mom >>=
                    (\gf2 -> return (gf1, gf2)))))

-- let g = (\gf2 -> return (gf1, gf2))
-- bothGrandfathers' p =
--     (father p >>= father) >>=
--         (\gf1 -> (mother p >>= father) >>=
--             (\gf2 -> pure (gf1, gf2)))
