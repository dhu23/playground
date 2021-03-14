add2 <- function(x, y)
{
  x+y
}

above10 <- function(x) 
{
  use <- x > 10
  x[use]
}

above <- function(x, n=10) 
{
  x[x>n]
}

columnmean <- function(y, removeNA = TRUE)
{
  nc <- ncol(y)
  means <- numeric(nc) # vector of size nc
  for(i in 1:nc)
  {
    means[i] <- mean(y[i], na.rm = removeNA)
  }
}


make.NegLogLikelihood <- function(data, fixed=c(FALSE, FALSE)) 
{
  params <- fixed
  function(p) 
  {
    params[!fixed] <- p
    mu <- params[1]
    sigma <- params[2]
    a <- -0.5*length(data)*log(2*pi*sigma^2)
    b <- -0.5*sum((data-mu)^2/(sigma^2))
    -(a+b)
  }
}