% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/saem_fit.R
\name{gen_saem_user_fn}
\alias{gen_saem_user_fn}
\title{Generate an SAEM model}
\usage{
gen_saem_user_fn(
  model,
  PKpars = attr(model, "default.pars"),
  pred = NULL,
  err = NULL,
  control = saemControl(),
  inPars = NULL
)
}
\arguments{
\item{model}{a compiled SAEM model by gen_saem_user_fn()}

\item{PKpars}{PKpars function}

\item{pred}{pred function;  This will be a focei-style pred}

\item{inPars}{a character vector of parameters to be read from the
input dataset (including time varying covariates)}
}
\description{
Generate an SAEM model using either closed-form solutions or ODE-based model definitions
}
\details{
Fit a generalized nonlinear mixed-effect model using the
  Stochastic Approximation Expectation-Maximization (SAEM)
  algorithm
}
\author{
Matthew Fidler & Wenping Wang
}
\keyword{internal}
