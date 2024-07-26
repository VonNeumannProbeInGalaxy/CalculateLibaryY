#include "math.h"
#include "CLY.h" 

//��ʽ��Դ�ڣ���������������� 

//�������ַ�����-������������ 
static double CMassofShip = 1145141919810.0;
static double CRadiusofMirror = 1145140.0/2; //�ɴ����侵�뾶 
static float CBLGk = 0.9998;//������ 
static float CTmax = 919.810;//���������¶�
//static double CMWet = 5.14;//E11�ɴ�ʪ��
static float DensityofMirror = 0.05;//�ⷫ��ƽ���ܶȣ����Ǹ�������֮�� 
static float ShipWeave = 0.0114514/3600; //���룺һǧ�׳߶ȵ������������Ϊ2.4���� 

//���ǻ����������� 
//��������뾶  
float StarsRadius(double mass)  ///��ϲ��� 
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

//������������ 
float Starslum(double mass) 
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
float StarsTemp(struct StarsSelf SSCP)   
{
	double LC4PK,R2SB;	
	double P4xA;
	P4xA = 4.0 *Pai;
	LC4PK = SSCP.lum * SunLight *E26;
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
float LaunchBlueShift(double PersentofC)  
{
	double UPOC,DWNOC,CALUMC;
	UPOC = PersentofC;
	DWNOC = PersentofC; 
	UPOC += 1.0;  
	DWNOC += 1.0;
	CALUMC = UPOC/DWNOC;
	UPOC = pow(CALUMC,0.5);
	return UPOC;
}
//����Ŀ������ĩ�ٶ�������Ч�ʹ�ϵ 
float LaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0-PersentofC;
	DWNOC = 1.0+PersentofC;
	CALUMC = UPOC/DWNOC;
	UPOC = 1.0-CALUMC;
	return UPOC;	
}
//ȫ������¹��������ͱ��������嶯�ܱ�ֵ����Ч�ʣ� 
float TotalLaserEfficiency(double PersentofC)
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
float TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//���ٻ��ѹ������������� 
float LandingCostEnergy(double PersentofC)
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
float MirrorRadiustoArea(double RadiusofMirror)
{
	double UPOC,DWNOC;
	UPOC = pow(RadiusofMirror,2);
	DWNOC = UPOC * Pai;
	return DWNOC; 
}
//���������ʼ��� 
float BraggReflection(float FilmN1,float FilmN2,int FilmNumber,float BaseFilmN)//��������Ĥ��1,2�����ʣ����������������� 
{
	//Ĭ�� ��� ��Ϊ���� -1.00-������ 
	float MediumRefractive = 1.00;
	float UPOC,DWNOC,CALUMC;
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
float TotalMinMirrowPower(float Tmax,float KBragg,double AreaofMirror)
{
	float UPOC,DWNOC,CALUMC;
	DWNOC = pow(Tmax,4);
	UPOC = StefanBoltzmannConst * DWNOC;
	CALUMC = 1.0 - KBragg;
	DWNOC = UPOC / CALUMC;
	CALUMC = DWNOC * AreaofMirror;
	return  CALUMC;
}
//�ⷫ��ѹ 
float PsailCalluate(float Tmax,float KBragg)
{
	float UPOC,DWNOC,CALUMC;
	DWNOC = pow(Tmax,4);
	UPOC = StefanBoltzmannConst * DWNOC;
	CALUMC = 1.0 - KBragg;
	DWNOC = UPOC / CALUMC;
	DWNOC = DWNOC*2;
	CALUMC = DWNOC / C;
	return CALUMC;
}
//�����ⷫ�����������ȣ��������Σ���Ҫ���飩 
float SingalSailMaxCable(double MaxMateralStrengh,float Acceleration,float MateralDensity,double MaxCableMass,double MaxWetMass)
{
	float UPOC,DWNOC,CALUMC;
	UPOC = MaxCableMass * MaxMateralStrengh;
	DWNOC = MateralDensity * Acceleration;
	CALUMC = MaxWetMass + MaxCableMass;
	DWNOC = DWNOC * CALUMC;
	CALUMC = UPOC / DWNOC;
	return CALUMC;
 } 
 //���Ǽʹⷫ����RKKV2 -�ⷫ�뺽��(0.2.2) 
 //���׺����Сֵ ����ʹ��̼��ά�ܼ��� 
 float SailMinNumDensity(float Runit,float InnerP)
 {
	float UPOC,DWNOC,CALUMC;
	UPOC = Runit * InnerP;
	DWNOC = DensityofCarbonPipe *2;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
} 
//�ⷫ��������
float SailMassCal(float Area)
{
	Area = DensityofMirror * Area;
	return Area;
 }
//�ɴ����ٶ�
float AccelerationCl(float Tmax,float KBragg,double AreaofMirror)
{
	float UPOC,DWNOC,CALUMC;
	UPOC = TotalMinMirrowPower(Tmax,KBragg,AreaofMirror);
	DWNOC = 2.0 - KBragg;
	CALUMC = UPOC *  DWNOC;
	UPOC = C*CMassofShip;//�ɴ�ȫ�� 
	CALUMC = CALUMC/UPOC;
	return CALUMC;
}
//����������ľ���������㡪��������پ���
float DistenseofAcceleration()
{
	float UPOC,DWNOC,CALUMC;
	UPOC = 360.0 * CRadiusofMirror;
	DWNOC = 2.0*Pai;
	CALUMC = DWNOC * ShipWeave;
	CALUMC = UPOC / CALUMC;
	return CALUMC;
}
//ĩ�ٶȼ���--�������� 
float MaxVCl()
{
	float UPOC,DWNOC,CALUMC;
	return CALUMC;
} 
