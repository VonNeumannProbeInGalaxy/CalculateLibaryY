#ifndef _CLY_H
#define _CLY_H
//�ÿ⽫�᳢��ʹ�þ����ٵ��ⲿ�⣬�������� Ⱥ�ļ� 

//���泣�� 
#define Pai 3.14159265358979323846				 
#define StefanBoltzmannConst 0.00000005670367    //˹����-������������
#define C 299792458.0                            //����
#define G 6.674e-11                              //
#define VacuumPermeability 1.256637061435173e-6  //��մŵ��� 
#define BoltzmannConstant 1.3806505e-23          //
#define MassofProton 1.6726231e-27               //
#define eV 1.602176565e-19                       //���Ӵ����������ӷ��뽹������
#define eThomsonSection 6.65e-29                 //���ӵ�Thomson����
//
#define SunLight 3.85e26                         //�� ̫�����书�� 
#define SunRadius 695500000                      //����10��8�η� ��λ�� 
#define SecondofYear 31536000.0                  //Ϊ�˲�ȥ������ͣ�ͳһʹ��double���ͼ��� 
#define AU 149597870691.0                        //���ĵ�λ��ֵ
#define MassofEarth 5.9742e24                    // 
#define MassofSun 1.9891e30                      //
#define ly 9454254955488000.0                    //���� 
//Ԥ����ֵ
#define DensityofCarbonPipe 2000.0

struct StarsSelf			  //�������ǻ��������� 
{
	unsigned int IdentifyNum;          //Ψһʶ���롪������ͳһ����������Ҫ��ӵĽṹ��������ֻ��0��4294967295 
	double lox,loy,loz;		           //��ά�ռ����ꡪ����Ҫһ������㡱  
	double lum,Teff;                   //��ǿ��,��Ч�¶ȣ�ֻ��Ҫ��һ�� 
	double mass,radius;  	           //���̫������,���̫���뾶�������� 
	float age,HeavyElement;            //�������䣬�ؽ�����ȣ�ע�⡰�ؽ��������� �� 
	unsigned int ally;	               //��Ӫ����	
	float Facmass,Powmass,Unusemass;   //���������������������δ�������� 
};

struct ShipType               //�����ɴ��ͺŽṹ��
{
	unsigned int IdentifyNum;
	float ShipMass;
	float MirrorRadius; 
	float Tmax;                        //��������¶�
	float MassWet;                     //���˹ⷫ�����ʪ��
	float MassDry;                     //���˹֮ⷫ��ĸ���
	float MirrorMass;                  //�ⷫ���� 
	float KBLG;                        //������ 
	float ShipWeave;                   //���룺һǧ�׳߶ȵ������������Ϊ2.4����  
	float DensityofMirror;             //
}; 

struct ShipsDefine            //�����ɴ������� 
{
	unsigned int IdentifyNum;          //
	unsigned int NumofShips;           //���ӽ������� 
	unsigned int SystemFrom;           //������һ������ϵ 
	unsigned int SystemTo;             //ǰ����һ������ϵ
	float ProgessPershent;             //������(�ٷֱȣ� 
	double PersentofC;                 //��ǰ�ٶ�
	float LaunchTime;                  //����ʱ�� 
	float TargetTime;                  //�ִ�ʱ��  
};

//�������� 
//���ǲ������㺯�� 
//����֤ 
double StarsTemp(struct StarsSelf SSCP); 
double Starslum(double mass); 
double StarsRadius(double mass);
//�ⷫ�ɴ����㺯�� 
//�д����� 
double LaunchBlueShift(double PersentofC);  
double LaserEfficiency(double PersentofC);
double TotalLaserEfficiency(double PersentofC);
double TargetVDemandDysonPower(double PersentofC);
double LandingCostEnergy(double PersentofC);
double MirrorRadiustoArea(double RadiusofMirror);
double BraggReflection(float FilmN1,float FilmN2,int FilmNumber,float BaseFilmN);
double TotalMinMirrowPower(float Tmax,float KBragg,double AreaofMirror);
double PsailCalluate(float Tmax,float KBragg);
double SingalSailMaxCable(double MaxMateralStrengh,float Acceleration,float MateralDensity,double MaxCableMass,double MaxWetMass);
double SailMinNumDensity(float Runit,float InnerP);
double SailMassCal(float Area);
double AccelerationCl(float Tmax,float KBragg,double AreaofMirror);
double DistenseofAcceleration(); 
double RasorTime(double VofEnd,double a,double Lengh_A);
double MaxVCl(double Lengh_A,double a); 
//Ŀǰ�����ɴ���RKKV�ĵڶ���ͼ�ĵ�2024������ 

#pragma message("CLY-Version-0.2.3")  ///�ڱ���������ʾ�汾��

#endif
