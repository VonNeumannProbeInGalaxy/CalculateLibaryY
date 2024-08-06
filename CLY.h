#ifndef _CLY_H
#define _CLY_H
//�ÿ⽫�᳢��ʹ�þ����ٵ��ⲿ�⣬�������� Ⱥ�ļ� 

//���泣�� 
#define Pai 3.14159265358979323846				 
#define StefanBoltzmannConst 0.00000005670367    //˹����-������������
#define C 299792458.0                            //����
#define G 6.674                                  //E-11
#define VacuumPermeability 1.256637061435173     //E-6 ��մŵ��� 
#define BoltzmannConstant 1.3806505              //E-23
#define MassofProton 1.6726231                   //E-27
#define eV 1.602176565                           //E-19 ���Ӵ����������ӷ��뽹������
#define eThomsonSection 6.65                     //E-29 ���ӵ�Thomson����
//
#define SunLight 3.85                         	 //E26 ��λ�� ̫�����书�� 
#define SunRadius 695500000                      //����10��8�η� ��λ�� 
#define SecondofYear 31536000.0                  //Ϊ�˲�ȥ������ͣ�ͳһʹ��double���ͼ��� 
#define AU 149597870691.0                        //���ĵ�λ��ֵ
#define MassofEarth 5.9742                       //E24 
#define MassofSun 1.9891                         //E30
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
float StarsTemp(struct StarsSelf SSCP); 
float Starslum(double mass); 
float StarsRadius(double mass);
//�ⷫ�ɴ����㺯�� 
//�д����� 
float LaunchBlueShift(double PersentofC);  
float LaserEfficiency(double PersentofC);
float TotalLaserEfficiency(double PersentofC);
float TargetVDemandDysonPower(double PersentofC);
float LandingCostEnergy(double PersentofC);
float MirrorRadiustoArea(double RadiusofMirror);
float BraggReflection(float FilmN1,float FilmN2,int FilmNumber,float BaseFilmN);
float TotalMinMirrowPower(float Tmax,float KBragg,double AreaofMirror);
float PsailCalluate(float Tmax,float KBragg);
float SingalSailMaxCable(double MaxMateralStrengh,float Acceleration,float MateralDensity,double MaxCableMass,double MaxWetMass);
float SailMinNumDensity(float Runit,float InnerP);
float SailMassCal(float Area);
float AccelerationCl(float Tmax,float KBragg,double AreaofMirror);
float DistenseofAcceleration(); 


#pragma message("CLY-Version-0.2.1")  ///�ڱ���������ʾ�汾��
