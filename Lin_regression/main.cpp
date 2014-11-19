// Finds line of regression

#include <vector>
#include <cstdlib>
#include <numeric>
#include <iostream>


using namespace std;

double slope(const vector<double>& y, const vector<double>& x){
     if(x.size() != y.size())
    {
        cout << "Two vectors are not equal " << endl;
    }

    double sigmax = 0;
    double sigmay = 0;
    double sigmaxsq = 0;
    double sigmaxy = 0;
    int n = x.size();
    for(int i = 0; i < n; i++)
    {
        sigmax += x[i];
        sigmay += y[i];
        sigmaxsq += x[i] * x[i];
        sigmaxy += x[i] * y[i];
    }

    double xavg = sigmax / x.size();
    double yavg = sigmay / y.size();

    double beta1top = 0;
    double beta1bottom = 0;

    double beta1 = (sigmaxy - n * xavg * yavg)/(sigmaxsq - n * xavg * xavg);
    double beta0 = yavg - beta1 * xavg;

  
    cout << "Beta0: " << beta0 << endl;
    cout << "Beta1: " << beta1 << endl;
}


main(int argc, char** argv) {

    vector<double> x;
    vector<double> y;
    
    y.push_back(130);
    y.push_back(650);
    y.push_back(99);
    y.push_back(150);
    y.push_back(128);
    y.push_back(302);
    y.push_back(95);
    y.push_back(945);
    y.push_back(368);
    y.push_back(961);
    x.push_back(163);
    x.push_back(765);
    x.push_back(141);
    x.push_back(166);
    x.push_back(137);
    x.push_back(355);
    x.push_back(136);
    x.push_back(1206);
    x.push_back(433);
    x.push_back(1130);
    slope(x,y);
    
    return 0;
}

