#ifndef __ITENSOR_SWEEPS_HEADER_H
#define __ITENSOR_SWEEPS_HEADER_H

class Sweeps
{
public:
    enum Scheme {ramp_m, fixed_m, fixed_cutoff};

    //Constructors --------------

    Sweeps();
    
    Sweeps(Scheme sch);

    Sweeps(Scheme sch, int nsw);

    Sweeps(Scheme sch, int nsw, int _minm, int _maxm, Real _cut);
    
    //Accessor methods ----------

    Scheme 
    scheme() const { return scheme_; }

    void 
    setScheme(Scheme val) { uninit(); scheme_ = val; }

    int 
    minm(int sw) const { return Minm_; }
    void 
    setMinm(int val) { uninit(); Minm_ = val; }

    int 
    maxm(int sw) const { init(); return Maxm_.at(sw); }
    void 
    setMaxm(int val) { uninit(); finalMaxm_ = val; }

    Real 
    cutoff(int sw) const { init(); return Cutoff_.at(sw); }
    void 
    setCutoff(Real val) { uninit(); finalCut_ = val; }

    int 
    nsweep() const { return Nsweep_; }
    void 
    setNsweep(int val) { uninit(); Nsweep_ = val; }

    int 
    niter(int sw) const { init(); return Niter_.at(sw); }

    int
    numSiteCenter() const { return num_site_center_; }

    void
    setNumSiteCenter(int val) { num_site_center_ = val; }

private:

    void 
    uninit() { init_ = false; }

    void 
    init() const;

private:
    Scheme scheme_;
    int Minm_, finalMaxm_;
    Real finalCut_;
    mutable std::vector<int>  Maxm_, Niter_;
    mutable std::vector<Real> Cutoff_;
    int Nsweep_;
    int num_site_center_;        // May not be implemented in some cases
    mutable bool init_;
};

inline Sweeps::
Sweeps()
: scheme_(ramp_m),
  Minm_(1), finalMaxm_(MAX_M), finalCut_(MIN_CUT),
  Nsweep_(0), num_site_center_(2)
{ }

inline Sweeps::
Sweeps(Scheme sch)
: scheme_(sch),
  Minm_(1), finalMaxm_(MAX_M), finalCut_(MIN_CUT),
  Nsweep_(0), num_site_center_(2)
{ }

inline Sweeps::
Sweeps(Scheme sch, int nsw, int _minm, int _maxm, Real _cut)
: scheme_(sch), 
  Minm_(_minm), finalMaxm_(_maxm), finalCut_(_cut),
  Maxm_(nsw+1), Niter_(nsw+1,4), Cutoff_(nsw+1), Nsweep_(nsw), 
  num_site_center_(2), init_(false)
{ }

inline Sweeps::
Sweeps(Scheme sch, int nsw)
: scheme_(sch),
  Minm_(1), finalMaxm_(MAX_M), finalCut_(MIN_CUT),
  Nsweep_(nsw), num_site_center_(2)
{ }

inline void Sweeps::
init() const
    {
    if(init_) return;

    Maxm_.resize(Nsweep_+1);
    Niter_.resize(Nsweep_+1);
    Cutoff_.resize(Nsweep_+1);

    if(scheme_ == ramp_m)
        {
        for(int s = 1; s <= Nsweep_; s++)
        { Cutoff_.at(s) = finalCut_; Maxm_.at(s) = (int)(Minm_ + (s-1.0)/Nsweep_ * (finalMaxm_ - Minm_)); }
        }
    else if(scheme_ == fixed_m || scheme_ == fixed_cutoff)
        {
        for(int s = 1; s <= Nsweep_; s++)
        { Cutoff_.at(s) = finalCut_; Maxm_.at(s) = finalMaxm_; }
        }
    
    for(int s = 1; s <= Nsweep_; s++)
        { 
        if(s <= min(Nsweep_,4))
            Niter_.at(s) = 10-s;
        else
            Niter_.at(s) = 4; 
        }

    init_ = true;

    } //Sweeps::init

inline void 
sweepnext(int &l, int &ha, int N, int min_l = 1)
    {
    if(ha == 1)
        {
        if(++l == N) 
            l = N-1, ha = 2;
        return;
        }
    if(l-- == min_l) ha = 3;
    }

#endif //__ITENSOR_SWEEPS_HEADER_H
