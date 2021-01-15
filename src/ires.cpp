#include "ires.h"

extern "C" SEXP _nlmixr_iresCalc(SEXP ipredDfLstSXP, SEXP dvIn, SEXP evidIn, SEXP censIn, SEXP limitIn,
				 SEXP iresOpt) {
BEGIN_RCPP
  List ipredL = as<List>(ipredDfLstSXP);
  int ncalc = Rf_length(ipredL[0]);

  int npred = getPredIndex(ipredL);

  arma::vec ipredt(REAL(ipredL[npred]), ncalc, false, true);
  arma::vec ipred(ipredt.size());

  arma::vec dv(REAL(dvIn), ncalc, false, true);
  arma::vec dvt(ncalc);

  arma::vec riv(REAL(ipredL[npred+1]), ncalc, false, true);


  arma::ivec cens;
  if (Rf_isNull(censIn)) {
    cens = arma::ivec(ncalc, fill::zeros);
  } else {
    cens = as<arma::ivec>(censIn);
  }
  arma::ivec evid;
  if (Rf_isNull(evidIn)) {
    evid = arma::ivec(ncalc, fill::zeros);
  } else {
    evid = as<arma::ivec>(evidIn);
  }

  arma::vec limit;
  int hasLimit=0;
  getLimitFromInput(limitIn, ncalc, limit, hasLimit);

  arma::vec     hi(REAL(ipredL[ipredL.size()-1]), ncalc, false, true);
  arma::vec    low(REAL(ipredL[ipredL.size()-2]), ncalc, false, true);
  arma::vec     yj(REAL(ipredL[ipredL.size()-3]), ncalc, false, true);
  arma::vec lambda(REAL(ipredL[ipredL.size()-4]), ncalc, false, true);
  arma::vec lowerLim(ncalc);
  arma::vec upperLim(ncalc);

  bool doSim = true;
  List opt = as<List>(iresOpt);
  if (opt.containsElementNamed("doSim")) {
    RObject tmp = opt["doSim"];
    if (TYPEOF(tmp) == LGLSXP) {
      doSim = as<bool>(tmp);
    }
  }
  int censMethod = CENS_TNORM;
  if (opt.containsElementNamed("censMethod")) {
    RObject tmp = opt["censMethod"];
    if (TYPEOF(tmp) == INTSXP) {
      censMethod = as<int>(opt["censMethod"]);
    }
  }

  bool interestingLimits = censTruncatedMvnReturnInterestingLimits(dv, dvt, ipred, ipredt, cens, limit,
  								   lambda, yj, low, hi, lowerLim, upperLim,
  								   riv, doSim);

  arma::ivec ID(INTEGER(ipredL[0]), ncalc, false, true);

  arma::vec iwres=(dvt-ipredt)/sqrt(riv);
  arma::vec ires = dv - ipred;

  for (unsigned int j = ires.size(); j--; ) {
    if (censMethod == CENS_OMIT && cens[j] != 0) {
      dv[j]	= NA_REAL;
      ipred[j]	= NA_REAL;
      ires[j]	= NA_REAL;
      iwres[j]	= NA_REAL;
    } else if (evid[j] != 0) {
      dv[j]	= NA_REAL;
      ires[j]	= NA_REAL;
      iwres[j]	= NA_REAL;
    }
  }
  DataFrame retDF;

  if (interestingLimits) {
    retDF = DataFrame::create(_["DV"]=wrap(dv),
			      _["IPRED"]=wrap(ipred),
			      _["IRES"]=wrap(ires),
			      _["IWRES"]=wrap(iwres),
			      _["lowerLim"] = wrap(lowerLim),
			      _["upperLim"] = wrap(upperLim));
  } else {
    retDF = DataFrame::create(_["DV"]=wrap(dv),
			      _["IPRED"]=wrap(ipred),
			      _["IRES"]=wrap(ires),
			      _["IWRES"]=wrap(iwres));
  }
  return wrap(retDF);
END_RCPP
}
