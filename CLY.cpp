#include "math.h"
#include "CLY.h" 

//公式来源于：达尔文黎明讨论组 

//妙妙数字放这里-仅供测试用例 
static double CMassofShip = 1145141919810.0;
static double CRadiusofMirror = 1145140.0/2; //飞船反射镜半径 
static double CBLGk = 0.9998;//反射率 
static double CTmax = 919.810;//接受能流温度
//static double CMWet = 5.14;//E11飞船湿重
static double DensityofMirror = 0.05;//光帆总平均密度，考虑更多死重之后 
static double ShipWeave = 0.0114514/3600; //角秒：一千米尺度的物体来回振幅为2.4毫米 

//恒星基本参数计算 
//现在恒星参数用MIST，不使用这里的拟合

//质量计算半径  
double StarsRadius(double mass)  ///拟合参数 
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

//质量计算亮度   后期可能要修改 
double Starslum(double mass) 
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

//恒星际光帆船与RKKV1 -功与速度(0.2.0) 

//根据速度计算波长-蓝移
double BlueShift(double beta)  
{
	//beta是v/c
	return sqrt((1.0 + beta) / (1.0 - beta));
}
//计算目标物体末速度与能量效率关系 
double LaserEfficiency(double beta)
{
	//beta是v/c，k是过程变量
	double k = (1.0 - beta) / (1.0 + beta);
	return double(1.0 - k);
}
//全程情况下光子能量和被加速物体动能比值（总效率） 
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
//速度与戴森球所需功率
double TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//减速花费光束总能量计算 
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
//恒星际光帆船与RKKV1 -光帆与航行(0.2.1) 
//光帆面积 
double MirrorRadiustoArea(double RadiusofMirror)
{
	double UPOC,DWNOC;
	UPOC = pow(RadiusofMirror,2);
	DWNOC = UPOC * Pai;
	return DWNOC; 
}
//布拉格反射率计算 
double BraggReflection(double FilmN1,double FilmN2,int FilmNumber,double BaseFilmN)//交替生长膜层1,2折射率，层数，基底折射率 
{
	//默认 真空 作为介质 -1.00-折射率 
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
//发射飞船所需功率 
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
//光帆内压 
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
//单个光帆极限缆绳长度（经过变形，需要检验） 
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
 //恒星际光帆船与RKKV2 -光帆与航行(0.2.2) 
 //帆底厚度最小值 ——使用碳纤维管计算 
double SailMinNumDensity(double Runit,double InnerP)
 {
	double UPOC,DWNOC,CALUMC;
	UPOC = Runit * InnerP;
	DWNOC = DensityofCarbonPipe *2;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
} 
//光帆质量计算
double SailMassCal(double Area)
{
	Area = DensityofMirror * Area;
	return Area;
 }
//飞船加速度
double AccelerationCl(double Tmax,double KBragg,double AreaofMirror)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = TotalMinMirrowPower(Tmax,KBragg,AreaofMirror);
	DWNOC = 2.0 - KBragg;
	CALUMC = UPOC *  DWNOC;
	UPOC = C*CMassofShip;//飞船全重 
	CALUMC = CALUMC/UPOC;
	return CALUMC;
}
//不考虑衍射的精度需求计算——计算加速距离
double DistenseofAcceleration()
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 360.0 * CRadiusofMirror;
	DWNOC = 2.0*Pai;
	CALUMC = DWNOC * ShipWeave;
	CALUMC = UPOC / CALUMC;
	return CALUMC;
}
//激光器开机时长
double RasorTime(double VofEnd,double a,double Lengh_A)//末速度，加速度，加速距离，加速距离在上一个式子中计算得出 
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
//末速度计算--经过化简（0.2.3） 
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

