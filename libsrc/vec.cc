// vec.cc: implementation of integer vector classes
//////////////////////////////////////////////////////////////////////////
//
// Copyright 1990-2012 John Cremona
// 
// This file is part of the eclib package.
// 
// eclib is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
// 
// eclib is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License
// along with eclib; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
// 
//////////////////////////////////////////////////////////////////////////
 
// Only to be included by vector.cc

// Definitions of member operators and functions:

vec::~vec()
{
  delete[] entries;
}

vec::vec(long n)
{
 d=n;
 entries=new scalar[n]; 
 if (!entries) {cout<<"Out of memory!\n"; abort();}
 scalar *v=entries; while(n--) *v++=0;
}

vec::vec(long n, scalar* arr)   //entries must have at least n elements!
{
 d=n;
 entries=new scalar[n];  
 if (!entries) {cout<<"Out of memory!\n"; abort();} 
 scalar *v=entries; while(n--) *v++=*arr++;
}

vec::vec(const vec& v)                       // copy constructor
{
  d=v.d;
  entries=new scalar[d];  
  if (!entries) {cout<<"Out of memory!\n"; abort();}
  scalar *v1=entries, *v2=v.entries; long n=d;
  while(n--) *v1++=*v2++;
}

void vec::init(long n)                 // (re)-initializes 
{
 if (d!=n) // no point in deleting if same size
   {
     delete[] entries;
     d = n;
     entries=new scalar[d];  
     if (!entries) {cout<<"Out of memory!\n"; abort();}
   }
 scalar *v=entries; while(n--) *v++=0;
}

vec& vec::operator=(const vec& v)                    // assignment
{
 if (this==&v) return *this;
 if (d!=v.d) // no point in deleting if new is same size
   {
     delete[] entries;
     d = v.d;
     entries=new scalar[d];  
     if (!entries) {cout<<"Out of memory!\n"; abort();}
   }
 scalar *v1=entries, *v2=v.entries; long n=d; 
 while(n--) *v1++=*v2++;
 return *this;
}

scalar& vec::operator[](long i) const
{
 if ((i>0) && (i<=d)) return entries[i-1];
 else {cout << "bad subscript in vec::operator[]\n"; abort(); return entries[0];}
}

vec& vec::operator+=(const vec& q2)
{
  scalar* vi=entries, *wi=q2.entries; long i=d;
  if (d==q2.d) {while(i--)(*vi++)+=(*wi++);}
  else {cout << "Incompatible vecs in vec::operator+=\n"; abort();}
  return *this;
}

void vec::addmodp(const vec& w, scalar pr)
{
  scalar* vi=entries, *wi=w.entries; long i=d;
  if (d==w.d) {while(i--) {*vi = xmod((*wi++)+(*vi),pr);vi++;}}
  else {cout << "Incompatible vecs in vec::addmodp\n"; abort(); }
}

vec& vec::operator-=(const vec& q2)
{
  scalar* vi=entries; scalar* wi=q2.entries; long i=d;
  if (d==q2.d) {while(i--)(*vi++)-=(*wi++);}
  else {cout << "Incompatible vecs in vec::operator-=\n"; abort();}
  return *this;
}

vec& vec::operator*=(scalar scal)
{
  scalar* vi=entries; long i=d;
  while (i--) (*vi++) *= scal;
  return *this;
}

vec& vec::operator/=(scalar scal)
{
  scalar* vi=entries; long i=d;
  while (i--) (*vi++) /= scal;
  return *this;
}

vec vec::slice(long first, long last) const       // returns subvector
{
 if (last==-1) {last=first; first=1;}
 long n = last-first+1;
 vec ans(n);
 scalar *veci=entries+(first-1), *ansi=ans.entries; long i=n;
 while (i--) *ansi++ = *veci++;
 return ans;
}

vec vec::operator[](const vec& index) const  // returns v[index[j]]
{long i=index.d; vec w(i);
 scalar* wi=w.entries, *indexi=index.entries;
 while (i--) (*wi++) = entries[(*indexi++)-1];
 return w;
}

scalar vec::sub(long i) const
{
 if ((i>0) && (i<=d)) return entries[i-1];
 else {cout << "bad subscript in vec::sub\n"; abort(); return 0;}
}

void vec::set(long i, scalar x)
{
 if ((i>0) && (i<=d)) entries[i-1]=x;
 else {cout << "bad subscript in vec::set\n"; abort(); }
}

void vec::add(long i, scalar x)
{
 if ((i>0) && (i<=d)) entries[i-1]+=x;
 else {cout << "bad subscript in vec::add\n"; abort(); }
}

// Definitions of non-member, friend operators and functions

scalar operator*(const vec& v, const vec& w)
{
 long dim=v.d, dot=0;
 scalar* vi=v.entries, *wi=w.entries;
 if (dim==w.d) 
   while (dim--) dot+= (*vi++)*(*wi++);
 else 
   {
     cout << "Unequal dimensions in dot product\n";
     abort();
   }
 return dot;
}

int operator==(const vec& v, const vec& w)
{
   long dim=v.d;
   long equal = (dim==w.d);
   scalar* vi=v.entries, *wi=w.entries;
   while ((dim--) && equal) equal = ((*vi++)==(*wi++));
   return equal;
}

int trivial(const vec& v)
{
   int ans=1, i=v.d;   scalar* vi=v.entries;
   while ((i--)&&ans) ans=((*vi++)==0);
   return ans;
}

ostream& operator<<(ostream& s, const vec& v)
{
   long i=v.d; scalar* vi=v.entries;
   s << "[";
   while (i--) {s<<(*vi++); if(i)s<<",";}
   s << "]";
   return s;
}

istream& operator>>(istream& s, vec& v)
{
 long i = v.d;
 scalar* vi = v.entries;
 while (i--) s >> (*vi++);
 return s;
}

vec iota(scalar n)
{
 vec v(n);
 scalar* entriesi=v.entries; long i=0;
 while (i<n) (*entriesi++)=(++i);
 return v;
}

scalar vecgcd(const vec& v)
{
 long i=v.d; 
 scalar g=0; 
 if (i==0) {g=1;} // so empty vector has content 1, not 0
 scalar *vi=v.entries;
 while ((i--)&&(g!=1)) g=gcd(g,*vi++);
 return g;
}

void swapvec(vec& v, vec& w)
{scalar *temp; 
 if (v.d==w.d) {temp=v.entries; v.entries=w.entries; w.entries=temp;}
 else {cout << "Attempt to swap vecs of different lengths!\n"; abort();}
}

int member(scalar a, const vec& v)
{int ans=0; long i=dim(v); scalar* vi=v.entries;
 while (i--&&!ans) ans=(a==(*vi++));
 return ans;
}
 
// Definition of non-friend operators and functions

vec reverse(vec& order)
{ long i,n = dim(order);
  vec ans(n);
  for (i=1; i<=n; i++) ans.set(order[i],i);
  return ans;
}
 
vec express(const vec& v, const vec& v1, const vec& v2)
{
   vec ans(3);
   scalar v1v1 = v1 * v1;
   scalar v1v2 = v1 * v2;
   scalar v2v2 = v2 * v2;
   scalar vv1 = v * v1;
   scalar vv2 = v * v2;
   ans[1]= vv1*v2v2 - vv2*v1v2;
   ans[2]= vv2*v1v1 - vv1*v1v2;
   ans[3]= v1v1*v2v2 - v1v2*v1v2;
   scalar g = vecgcd(ans);
   if (g>1) ans/=g;
   if (ans[3]*v!=ans[1]*v1+ans[2]*v2)
     {
       cout << "Error in express: v is not in <v1,v2>\n";
       abort();
     }
   return ans;
}

#if(0) // simple version of lift

int lift(const vec& v, scalar pr, vec& ans)
{
 long i, d = dim(v);
 ans =vec(d);
 int success, succ;
 float lim = sqrt(pr/2.0);
 scalar g, nu, de;  // = least common denom. after lifting via modrat
 for (i=1, g=1, success=1; i<=d; i++) 
   {
     succ = modrat(ans[i],pr,lim,nu,de); de=abs(de);
     success = success && succ;
     g=lcm(g,de);
   }
 for (i=1; i<=d; i++) ans[i] = mod(xmodmul(g,ans[i],pr),pr);
//Repeat if any failures were found
 if(!success)
   {
     for (i=1, g=1, success=1; i<=d; i++) 
       {
	 succ = modrat(ans[i],pr,lim,nu,de); de=abs(de);
	 success = success && succ;
	 g=lcm(g,de);
       }
     for (i=1; i<=d; i++) ans[i] = mod(xmodmul(g,ans[i],pr),pr);
   }
 if(!success)
   {
     //cout << "vec failed to lift from mod " << pr << " after two rounds.\n";
     return 0;
   }
 return 1;
}

#else
//#define LIFT_DEBUG
int lift(const vec& v, scalar pr, vec& ans)
{
  long i0, i, j, d = dim(v);
 scalar nu, de;
 int succ;
 float lim = sqrt(pr/2.0)-1;
 scalar maxallowed = 10*int(lim);
#ifdef LIFT_DEBUG
 cout<<"Lifting vector v = "<<v<<endl;
#endif
 // NB We do *not* make cumulative rescalings, since it is possible
 // for an apparently successful modrat reconstruction to give an
 // incorrect denominator.  I have an example with pr=2^30-35 where
 // the correct denominator is 4666 and one entry of the correct
 // primitive scaled vector is 47493 (greater than lim = 23170) but
 // since 47493/4666 = 587037152 = -10193/21607 (mod pr), rational
 // reconstruction returned nu=-10193, de = 21607.  If we kept the
 // (unsuccessful) scaling by 21607, all subsequent numerators would
 // be multiplied by this and we would never succeed.

 // This code allows for some entries to be >lim, and works as long as
 // (1) there is a lift with all entried at most 10*lim, (2) at least
 // one entry has the correct denominator, which is equaivalent to
 // requiring that in the primitive rescaling, there is an entry
 // coprime to the first non-zero entry.

 ans = v; // starts as a copy, and will be rescaled in place
 scalar vi0, inv_vi0, vi, maxvi=0;
 for(i0=1; i0<=d; i0++)
   {
     // scale so that i0'th entry is 1 mod p, then reduce vector
     // entries mod p to lie in (-p/2,p/2), and find the maximum
     // entry:
     while((vi0=mod(v[i0],pr))==0) {i0++;} // skip over any zero entries
     inv_vi0=invmod(vi0,pr);
     for (i=1; i<=d; i++)
       {
         ans[i]=vi=mod(xmodmul(inv_vi0,ans[i],pr),pr);
         maxvi=max(maxvi,abs(vi));
       }
#ifdef LIFT_DEBUG
     cout<<"Reduced v = "<<ans<<", with max entry "<<maxvi<<endl;
#endif
     if(maxvi<=maxallowed) // no scaling needed!
           {
             // Normalize so first nonzero entry is positive:
             for(i0=1; i0<=d; i0++)
               {
                 while(ans[i0]==0) {i0++;}
                 if(ans[i0]<0) ans=-ans;
                 return 1;
               }
             return 0; // should not happen: means v==0!
           }

     for(i=1; (i<=d); i++)
       {
         succ=modrat(ans[i],pr,lim,nu,de);     de=abs(de);
         if ((!succ)||(de==1)) continue; // loop on i
         // scale by de & recompute max entry:
#ifdef LIFT_DEBUG
         cout<<"Scaling by d="<<de<<endl;
#endif
         maxvi = 0;
         for (j=1; j<=d; j++) 
           {
             ans[j] = vi = mod(xmodmul(de,ans[j],pr),pr);
             maxvi=max(maxvi,abs(vi));
           }
#ifdef LIFT_DEBUG
         cout<<"Now v = "<<ans<<", with max entry "<<maxvi<<endl;
#endif
         if(maxvi<=maxallowed)
           {
             // Normalize so first nonzero entry is positive:
             for(i0=1; i0<=d; i0++)
               {
                 while(ans[i0]==0) {i0++;}
                 if(ans[i0]<0) ans=-ans;
                 return 1;
               }
             return 0; // should not happen: means v==0!
           }
       }
   }
 // Normalize so first nonzero entry is positive:
 for(i0=1; i0<=d; i0++)
   {
     while(ans[i0]==0) {i0++;}
     if(ans[i0]<0) ans=-ans;
     return (maxvi<=lim);
   }
 return 0;
}
#endif

#if(0) // old version
int old_liftok(vec& v, scalar pr)
{
 long i, d = dim(v);
 scalar g, nu, de; 
 int success, succ;
 float lim = sqrt(pr/2.0)-1;
 // scale vector so that first non-zero entry is 1
 i=1; while(mod(v[i],pr)==0) i++;
 scalar ivi = invmod(v[i],pr); v[i]=1; i++;
 for (; i<=d; i++) v[i]=mod(xmodmul(ivi,v[i],pr),pr);

 for (i=1, g=1, success=1; i<=d; i++) 
   {
     succ=modrat(v[i],pr,lim,nu,de); de=abs(de);
     success = success && succ;
// Can't say success=success&&modrat(...) as then after first fail it does
// not call modrat at all due to clever compiler!
     g=lcm(g,de);
//     if(succ&&(de>1))cout<<"Found denom of "<<de<<" from "<<v[i]<<", new g = "<<g<<"; ";
   }
 if(!success) 
   {
     cout << "modrat problems encountered lifting vector:\n";
     cout << v << "\n from mod " << pr << endl;
     cout << "Using denom = " << g << endl;
   }
 for (i=1; i<=d; i++) v[i] = mod(xmodmul(g,v[i],pr),pr);
//  if(!success) // have another go
//    {
//      for (i=1, g=1, success=1; i<=d; i++) 
//        {
// 	 succ=modrat(v[i],pr,lim,nu,de); de=abs(de);
// 	 success = success && succ;
// 	 g=lcm(g,de);
//        }
//      for (i=1; i<=d; i++) v[i] = mod(xmodmul(g,v[i],pr),pr);
//    }
 if(!success) 
   {
     cout << "returning vector:\n";
     cout << v << endl;
   }
 return success;
}
#endif

scalar dotmodp(const vec& v1, const vec& v2, scalar pr)
{
  scalar ans=0;
  for(long i=1; i<=dim(v1); i++) ans=mod(ans+mod(v1[i]*v2[i],pr),pr);
  return ans;
}
