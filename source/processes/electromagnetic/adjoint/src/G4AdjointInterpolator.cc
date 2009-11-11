#include "G4AdjointCSMatrix.hh"
#include "G4AdjointInterpolator.hh"

G4AdjointInterpolator* G4AdjointInterpolator::theInstance = 0;
///////////////////////////////////////////////////////
//
G4AdjointInterpolator* G4AdjointInterpolator::GetAdjointInterpolator()
{ if(theInstance == 0) {
    static G4AdjointInterpolator interpolator;
     theInstance = &interpolator;
  }
 return theInstance; 
}
///////////////////////////////////////////////////////
//
G4AdjointInterpolator* G4AdjointInterpolator::GetInstance()
{ if(theInstance == 0) {
    static G4AdjointInterpolator interpolator;
     theInstance = &interpolator;
  }
 return theInstance; 
}

///////////////////////////////////////////////////////
//
G4AdjointInterpolator::G4AdjointInterpolator()
{;
}
///////////////////////////////////////////////////////
//
G4AdjointInterpolator::~G4AdjointInterpolator()
{;
}
///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::LinearInterpolation(G4double& x,G4double& x1,G4double& x2,G4double& y1,G4double& y2)
{ G4double res = y1+ (x-x1)*(y2-y1)/(x2-x1);
  //G4cout<<"Linear "<<res<<std::endl;
  return res;	
}
///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::LogarithmicInterpolation(G4double& x,G4double& x1,G4double& x2,G4double& y1,G4double& y2)
{ if (y1<=0 || y2<=0 || x1<=0) return LinearInterpolation(x,x1,x2,y1,y2);
  G4double B=std::log(y2/y1)/std::log(x2/x1);
  //G4cout<<"x1,x2,y1,y2 "<<x1<<'\t'<<x2<<'\t'<<y1<<'\t'<<y2<<'\t'<<std::endl;
  G4double A=y1/std::pow(x1,B);
  G4double res=A*std::pow(x,B);
 // G4cout<<"Log "<<res<<std::endl;
  return res;
}
///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::ExponentialInterpolation(G4double& x,G4double& x1,G4double& x2,G4double& y1,G4double& y2)
{ G4double B=(std::log(y2)-std::log(y1));
  B=B/(x2-x1);
  G4double A=y1*std::exp(-B*x1);
  G4double res=A*std::exp(B*x);
  return res;
}
///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::Interpolation(G4double& x,G4double& x1,G4double& x2,G4double& y1,G4double& y2,G4String InterPolMethod)
{
  if (InterPolMethod == "Log" ){
  	return LogarithmicInterpolation(x,x1,x2,y1,y2);
  }
  else if (InterPolMethod == "Lin" ){
  	return LinearInterpolation(x,x1,x2,y1,y2);
  }
  else if (InterPolMethod == "Exp" ){
  	return ExponentialInterpolation(x,x1,x2,y1,y2);
  }
  else {
  	//G4cout<<"The interpolation method that you invoked does not exist!"<<std::endl; 
	return -1111111111.;
  }
}
///////////////////////////////////////////////////////
//
size_t  G4AdjointInterpolator::FindPosition(G4double& x,std::vector<double>& x_vec,size_t , size_t ) //only valid if x_vec is monotically increasing
{  //most rapid nethod could be used probably
   //It is important to put std::vector<double>& such that the vector itself is used and not a copy
  
  
  size_t ndim = x_vec.size();
  size_t ind1 = 0;
  size_t ind2 = ndim - 1;
 /* if (ind_max >= ind_min){
  	ind1=ind_min;
	ind2=ind_max;
	
  
  }
  */
  
  
  if (ndim >1) {
  	
	if (x_vec[0] < x_vec[1] ) { //increasing
		do {
         		size_t midBin = (ind1 + ind2)/2;
         		if (x < x_vec[midBin])
           				ind2 = midBin;
         		else 
					ind1 = midBin;
      		} while (ind2 - ind1 > 1);
	}
	else {
		do {
         		size_t midBin = (ind1 + ind2)/2;
         		if (x < x_vec[midBin])
           				ind1 = midBin;
         		else 
					ind2 = midBin;
      		} while (ind2 - ind1 > 1);
	}
  
  }
	
  return ind1;
}

///////////////////////////////////////////////////////
//
size_t  G4AdjointInterpolator::FindPositionForLogVector(G4double& log_x,std::vector<double>& log_x_vec) //only valid if x_vec is monotically increasing
{  //most rapid nethod could be used probably
   //It is important to put std::vector<double>& such that the vector itself is used and not a copy
  return FindPosition(log_x, log_x_vec);
  if (log_x_vec.size()>3){ 
  	size_t ind=0;
  	G4double log_x1=log_x_vec[1];
  	G4double d_log =log_x_vec[2]-log_x1;
	G4double dind=(log_x-log_x1)/d_log +1.;
	if (dind <1.) ind=0;
	else if (dind >= double(log_x_vec.size())-2.) ind =log_x_vec.size()-2;
	else ind =size_t(dind);
	return ind;
	
  }
  else 	return FindPosition(log_x, log_x_vec);
  
 
}
///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::Interpolate(G4double& x,std::vector<double>& x_vec,std::vector<double>& y_vec,G4String InterPolMethod)
{ size_t i=FindPosition(x,x_vec);
  //G4cout<<i<<std::endl;
  //G4cout<<x<<std::endl;
  //G4cout<<x_vec[i]<<std::endl; 
  return Interpolation(	x,x_vec[i],x_vec[i+1],y_vec[i],y_vec[i+1],InterPolMethod);
}

///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::InterpolateWithIndexVector(G4double& x,std::vector<double>& x_vec,std::vector<double>& y_vec,
					    std::vector<size_t>& index_vec,G4double x0, G4double dx) //only linear interpolation possible
{ size_t ind=0;
  if (x>x0) ind=int((x-x0)/dx);
  if (ind >= index_vec.size()-1) ind= index_vec.size()-2;
  size_t ind1 = index_vec[ind];
  size_t ind2 = index_vec[ind+1];
  if (ind1 >ind2) {
  	size_t ind11=ind1;
  	ind1=ind2;
	ind2=ind11;
  
  }
  
  ind=FindPosition(x,x_vec,ind1,ind2);
  return Interpolation(	x,x_vec[ind],x_vec[ind+1],y_vec[ind],y_vec[ind+1],"Lin");
  
}					    
					      

///////////////////////////////////////////////////////
//
G4double G4AdjointInterpolator::InterpolateForLogVector(G4double& log_x,std::vector<double>& log_x_vec,std::vector<double>& log_y_vec)
{ //size_t i=0;
  size_t i=FindPositionForLogVector(log_x,log_x_vec);
  /*G4cout<<"In interpolate "<<std::endl;
  G4cout<<i<<std::endl;
  G4cout<<log_x<<std::endl;
  G4cout<<log_x_vec[i]<<std::endl;
  G4cout<<log_x_vec[i+1]<<std::endl;
  G4cout<<log_y_vec[i]<<std::endl;
  G4cout<<log_y_vec[i+1]<<std::endl;*/
  
  G4double log_y=LinearInterpolation(log_x,log_x_vec[i],log_x_vec[i+1],log_y_vec[i],log_y_vec[i+1]);
  return log_y; 
 
}  
