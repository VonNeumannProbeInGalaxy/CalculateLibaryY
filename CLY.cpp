#include "math.h"
#include "CLY.h" 

//��ʽ��Դ�ڣ���������������� 

//�������ַ�����-������������ 
static double CMassofShip = 1145141919810.0;
static double CRadiusofMirror = 1145140.0/2; //�ɴ����侵�뾶 
static double CBLGk = 0.9998;//������ 
static double CTmax = 919.810;//���������¶�
//static double CMWet = 5.14;//E11�ɴ�ʪ��
static double DensityofMirror = 0.05;//�ⷫ��ƽ���ܶȣ����Ǹ�������֮�� 
static double ShipWeave = 0.0114514/3600; //���룺һǧ�׳߶ȵ������������Ϊ2.4���� 

//���ǻ����������� 
//���ں��ǲ�����MIST����ʹ����������

//��������뾶  
double StarsRadius(double mass)  ///��ϲ��� 
{
	double radius;
	double Xdss,Rdss,Kdss;
	Xdss = pow(1.4,0.9);
	Rdss = pow(1.4,0.6);
	Kdss = Xdss/Rdss;//
	Kdss = 1.10621;
	if(mass <= 1.4)/////ע�⣬0.2-30.0��̫����������Ĳ����ǣ�0.9���µ����Ч������
	{
		radius = pow(mass,0.9);
	}
	else
	{
		Xdss = pow(mass,0.6);
		radius = Kdss * Xdss;
	}
	return radius;
}

//������������   ���ڿ���Ҫ�޸� 
double Starslum(double mass) 
{
	double lum;
	double XdSs,RdSs;
	if(mass <= 0.43)
	{
		//���̫���������� 
		 XdSs = pow(mass,2.3);
		 RdSs = XdSs*0.23;
		 //̫������Ϊ1.0��ֱ����� 
		 lum = RdSs;
	}
	else if(mass <= 2.0)
	{
		XdSs = pow(mass,4);
		lum  = XdSs;
	}
	else if(mass <= 20.0)
	{
		XdSs = pow(mass,3.5);
		RdSs = 1.5*XdSs;
		lum = RdSs;
	}
	else
	{
		lum = 3200.0*mass;
	}
	return lum;
}

///�������̫�������͹�ȼ����Ч�¶�
double StarsTemp(struct StarsSelf SSCP)   
{
	double LC4PK,R2SB;	
	double P4xA;
	P4xA = 4.0 *Pai;
	LC4PK = SSCP.lum * SunLight;
	LC4PK = LC4PK / P4xA;
	R2SB = SSCP.radius * SunRadius;
	R2SB = pow(R2SB,2);
	R2SB = R2SB * StefanBoltzmannConst;
	LC4PK = LC4PK / R2SB;
	P4xA = pow(LC4PK,0.25);
	R2SB = P4xA;
	return R2SB;
}

//���Ǽʹⷫ����RKKV1 -�����ٶ�(0.2.0) 

//�����ٶȼ��㲨��-����
double BlueShift(double beta)  
{
	//beta��v/c
	return sqrt((1.0 + beta) / (1.0 - beta));
}
//����Ŀ������ĩ�ٶ�������Ч�ʹ�ϵ 
double LaserEfficiency(double beta)
{
	//beta��v/c��k�ǹ��̱���
	double k = (1.0 - beta) / (1.0 + beta);
	return double(1.0 - k);
}
//ȫ������¹��������ͱ��������嶯�ܱ�ֵ����Ч�ʣ� 
double TotalLaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	CALUMC = pow(PersentofC,2.0);
	UPOC = 1.0-CALUMC;
	DWNOC = pow(UPOC,0.5);
	UPOC = 2.0 * PersentofC;
	DWNOC += 1.0 + PersentofC;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
}
//�ٶ����ɭ�����蹦��
double TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//���ٻ��ѹ������������� 
double LandingCostEnergy(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	DWNOC = 1.0+PersentofC;
	UPOC = 2/DWNOC;
	UPOC -= 1.0;
	DWNOC = pow(UPOC,0.5);
	UPOC = 1.0-DWNOC;
	CALUMC = 0.5 *UPOC;
	return CALUMC;
}
//���Ǽʹⷫ����RKKV1 -�ⷫ�뺽��(0.2.1) 
//�ⷫ��� 
double MirrorRadiustoArea(double RadiusofMirror)
{
	double UPOC,DWNOC;
	UPOC = pow(RadiusofMirror,2);
	DWNOC = UPOC * Pai;
	return DWNOC; 
}
//���������ʼ��� 
double BraggReflection(double FilmN1,double FilmN2,int FilmNumber,double BaseFilmN)//��������Ĥ��1,2�����ʣ����������������� 
{
	//Ĭ�� ��� ��Ϊ���� -1.00-������ 
	double MediumRefractive = 1.00;
	double UPOC,DWNOC,CALUMC;
	UPOC = pow(FilmN2,2*FilmNumber);
	DWNOC = pow(FilmN1,2*FilmNumber);
	CALUMC = UPOC *MediumRefractive;
	UPOC = DWNOC *BaseFilmN;
	DWNOC = CALUMC - UPOC;
	UPOC = CALUMC + UPOC;
	DWNOC = DWNOC / UPOC;
	CALUMC = pow(DWNOC,2);
	return CALUMC;
}
//����ɴ����蹦�� 
double TotalMinMirrowPower(double Tmax,double KBragg,double AreaofMirror)
{
	double UPOC,DWNOC,CALUMC;
	DWNOC = pow(Tmax,4);
	UPOC = StefanBoltzmannConst * DWNOC;
	CALUMC = 1.0 - KBragg;
	DWNOC = UPOC / CALUMC;
	CALUMC = DWNOC * AreaofMirror;
	return  CALUMC;
}
//�ⷫ��ѹ 
double PsailCalluate(double Tmax,double KBragg)
{
	double UPOC,DWNOC,CALUMC;
	DWNOC = pow(Tmax,4);
	UPOC = StefanBoltzmannConst * DWNOC;
	CALUMC = 1.0 - KBragg;
	DWNOC = UPOC / CALUMC;
	DWNOC = DWNOC*2;
	CALUMC = DWNOC / C;
	return CALUMC;
}
//�����ⷫ�����������ȣ��������Σ���Ҫ���飩 
double SingalSailMaxCable(double MaxMateralStrengh,double Acceleration,double MateralDensity,double MaxCableMass,double MaxWetMass)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = MaxCableMass * MaxMateralStrengh;
	DWNOC = MateralDensity * Acceleration;
	CALUMC = MaxWetMass + MaxCableMass;
	DWNOC = DWNOC * CALUMC;
	CALUMC = UPOC / DWNOC;
	return CALUMC;
 } 
 //���Ǽʹⷫ����RKKV2 -�ⷫ�뺽��(0.2.2) 
 //���׺����Сֵ ����ʹ��̼��ά�ܼ��� 
double SailMinNumDensity(double Runit,double InnerP)
 {
	double UPOC,DWNOC,CALUMC;
	UPOC = Runit * InnerP;
	DWNOC = DensityofCarbonPipe *2;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
} 
//�ⷫ��������
double SailMassCal(double Area)
{
	Area = DensityofMirror * Area;
	return Area;
 }
//�ɴ����ٶ�
double AccelerationCl(double Tmax,double KBragg,double AreaofMirror)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = TotalMinMirrowPower(Tmax,KBragg,AreaofMirror);
	DWNOC = 2.0 - KBragg;
	CALUMC = UPOC *  DWNOC;
	UPOC = C*CMassofShip;//�ɴ�ȫ�� 
	CALUMC = CALUMC/UPOC;
	return CALUMC;
}
//����������ľ���������㡪��������پ���
double DistenseofAcceleration()
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 360.0 * CRadiusofMirror;
	DWNOC = 2.0*Pai;
	CALUMC = DWNOC * ShipWeave;
	CALUMC = UPOC / CALUMC;
	return CALUMC;
}
//����������ʱ��
double RasorTime(double VofEnd,double a,double Lengh_A)//ĩ�ٶȣ����ٶȣ����پ��룬���پ�������һ��ʽ���м���ó� 
{
	double UPOC,DWNOC,CALUMC;
	UPOC = C*VofEnd;
	DWNOC = pow(C,2);
	CALUMC = pow(VofEnd,2);
	DWNOC = DWNOC - CALUMC;
	CALUMC = pow(DWNOC,0.5);
	DWNOC = a * CALUMC;
	CALUMC = UPOC/DWNOC;
	UPOC = Lengh_A/C;
	CALUMC -= UPOC;
	return CALUMC;
 } 
//ĩ�ٶȼ���--��������0.2.3�� 
double MaxVCl(double Lengh_A,double a)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = pow(Lengh_A,2);
	UPOC = -1.0*UPOC;
	DWNOC = pow(a,2);
	UPOC = UPOC*DWNOC;
	DWNOC = pow(C,2);
	DWNOC = DWNOC *a;
	UPOC -= DWNOC;
	DWNOC = pow(C,4);
	UPOC += DWNOC;
	DWNOC = UPOC -1.00;
	CALUMC = UPOC/DWNOC;
	UPOC = pow(C,2);
	CALUMC = CALUMC*UPOC;
	CALUMC = pow(CALUMC,0.5);
	return CALUMC;
} 

