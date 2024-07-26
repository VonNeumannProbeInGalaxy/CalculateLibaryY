#include "math.h"
#include "CLY.h" 

//公式来源于：达尔文黎明讨论组 

//妙妙数字放这里-仅供测试用例 
static double CMassofShip = 1145141919810.0;
static double CRadiusofMirror = 1145140.0/2; //飞船反射镜半径 
static float CBLGk = 0.9998;//反射率 
static float CTmax = 919.810;//接受能流温度
//static double CMWet = 5.14;//E11飞船湿重
static float DensityofMirror = 0.05;//光帆总平均密度，考虑更多死重之后 
static float ShipWeave = 0.0114514/3600; //角秒：一千米尺度的物体来回振幅为2.4毫米 

//恒星基本参数计算 
//质量计算半径  
float StarsRadius(double mass)  ///拟合参数 
{
	double radius;
	double Xdss,Rdss,Kdss;
	Xdss = pow(1.4,0.9);
	Rdss = pow(1.4,0.6);
	Kdss = Xdss/Rdss;//
	Kdss = 1.10621;
	if(mass <= 1.4)/////注意，0.2-30.0个太阳质量以外的不考虑，0.9以下的拟合效果更好
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

//质量计算亮度 
float Starslum(double mass) 
{
	double lum;
	double XdSs,RdSs;
	if(mass <= 0.43)
	{
		//相对太阳质量比例 
		 XdSs = pow(mass,2.3);
		 RdSs = XdSs*0.23;
		 //太阳亮度为1.0故直接输出 
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

///根据相对太阳质量和光度计算等效温度
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

//恒星际光帆船与RKKV1 -功与速度(0.2.0) 
//根据速度计算波长-蓝移
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
//计算目标物体末速度与能量效率关系 
float LaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0-PersentofC;
	DWNOC = 1.0+PersentofC;
	CALUMC = UPOC/DWNOC;
	UPOC = 1.0-CALUMC;
	return UPOC;	
}
//全程情况下光子能量和被加速物体动能比值（总效率） 
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
//速度与戴森球所需功率
float TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//减速花费光束总能量计算 
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
//恒星际光帆船与RKKV1 -光帆与航行(0.2.1) 
//光帆面积 
float MirrorRadiustoArea(double RadiusofMirror)
{
	double UPOC,DWNOC;
	UPOC = pow(RadiusofMirror,2);
	DWNOC = UPOC * Pai;
	return DWNOC; 
}
//布拉格反射率计算 
float BraggReflection(float FilmN1,float FilmN2,int FilmNumber,float BaseFilmN)//交替生长膜层1,2折射率，层数，基底折射率 
{
	//默认 真空 作为介质 -1.00-折射率 
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
//发射飞船所需功率 
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
//光帆内压 
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
//单个光帆极限缆绳长度（经过变形，需要检验） 
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
 //恒星际光帆船与RKKV2 -光帆与航行(0.2.2) 
 //帆底厚度最小值 ——使用碳纤维管计算 
 float SailMinNumDensity(float Runit,float InnerP)
 {
	float UPOC,DWNOC,CALUMC;
	UPOC = Runit * InnerP;
	DWNOC = DensityofCarbonPipe *2;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
} 
//光帆质量计算
float SailMassCal(float Area)
{
	Area = DensityofMirror * Area;
	return Area;
 }
//飞船加速度
float AccelerationCl(float Tmax,float KBragg,double AreaofMirror)
{
	float UPOC,DWNOC,CALUMC;
	UPOC = TotalMinMirrowPower(Tmax,KBragg,AreaofMirror);
	DWNOC = 2.0 - KBragg;
	CALUMC = UPOC *  DWNOC;
	UPOC = C*CMassofShip;//飞船全重 
	CALUMC = CALUMC/UPOC;
	return CALUMC;
}
//不考虑衍射的精度需求计算——计算加速距离
float DistenseofAcceleration()
{
	float UPOC,DWNOC,CALUMC;
	UPOC = 360.0 * CRadiusofMirror;
	DWNOC = 2.0*Pai;
	CALUMC = DWNOC * ShipWeave;
	CALUMC = UPOC / CALUMC;
	return CALUMC;
}
//末速度计算--经过化简 
float MaxVCl()
{
	float UPOC,DWNOC,CALUMC;
	return CALUMC;
} 
