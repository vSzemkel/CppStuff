typedef pi T; // by Benq
T operator+(const T& l, const T& r) {
	return l.f != r.f ? min(l,r) : T{l.f,l.s+r.s}; }

const int SZ = ;
struct LazySeg { 
	const T ID = {MOD,0}; T comb(T a, T b) { return a+b; }
	T seg[2*SZ]; int lazy[2*SZ]; 
	LazySeg() { 
		F0R(i,2*SZ) seg[i] = {0,0}, lazy[i] = 0; 
		FOR(i,SZ,2*SZ) seg[i] = {0,1};
	}
	void push(int ind, int L, int R) { /// modify values for current node
		if (L != R) F0R(i,2) lazy[2*ind+i] += lazy[ind]; /// prop to children
		seg[ind].f += lazy[ind]; // dependent on operation
		lazy[ind] = 0; 
	} // recalc values for current node
	void pull(int ind) { seg[ind] = comb(seg[2*ind],seg[2*ind+1]); }
	void build() { ROF(i,1,SZ) pull(i); }
	void upd(int lo,int hi,int inc,int ind=1,int L=0, int R=SZ-1) {
		push(ind,L,R); if (hi < L || R < lo) return;
		if (lo <= L && R <= hi) { 
			lazy[ind] = inc; push(ind,L,R); return; }
		int M = (L+R)/2; upd(lo,hi,inc,2*ind,L,M); 
		upd(lo,hi,inc,2*ind+1,M+1,R); pull(ind);
	}
};
