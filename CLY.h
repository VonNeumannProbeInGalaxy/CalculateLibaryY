#ifndef _CLY_H
#define _CLY_H
//该库将会尝试使用尽量少的外部库，数据来自 群文件 

//宇宙常量 
#define Pai 3.14159265358979323846				 
#define StefanBoltzmannConst 0.00000005670367    //斯特潘-波尔兹曼常数
#define C 299792458.0                            //光速
#define G 6.674                                  //E-11
#define VacuumPermeability 1.256637061435173     //E-6 真空磁导率 
#define BoltzmannConstant 1.3806505              //E-23
#define MassofProton 1.6726231                   //E-27
#define eV 1.602176565                           //E-19 电子带电量、电子伏与焦耳换算
#define eThomsonSection 6.65                     //E-29 电子的Thomson截面
//
#define SunLight 3.85                         	 //E26 单位瓦 太阳辐射功率 
#define SunRadius 695500000                      //乘以10的8次方 单位米 
#define SecondofYear 31536000.0                  //为了不去辨别类型，统一使用double类型计算 
#define AU 149597870691.0                        //天文单位数值
#define MassofEarth 5.9742                       //E24 
#define MassofSun 1.9891                         //E30
#define ly 9454254955488000.0                    //光年 
//预设数值
#define DensityofCarbonPipe 2000.0

struct StarsSelf			  //描述恒星基本物理量 
{
	unsigned int IdentifyNum;          //唯一识别码——用作统一其他可能需要添加的结构体描述，只有0～4294967295 
	double lox,loy,loz;		           //三维空间坐标——需要一个“零点”  
	double lum,Teff;                   //光强度,等效温度，只需要算一次 
	double mass,radius;  	           //相对太阳质量,相对太阳半径——倍数 
	float age,HeavyElement;            //恒星年龄，重金属丰度（注意“重金属”定义 ） 
	unsigned int ally;	               //阵营编码	
	float Facmass,Powmass,Unusemass;   //工厂质量，发电机质量，未利用质量 
};

struct ShipType               //描述飞船型号结构体
{
	unsigned int IdentifyNum;
	float ShipMass;
	float MirrorRadius; 
	float Tmax;                        //最高耐受温度
	float MassWet;                     //除了光帆以外的湿重
	float MassDry;                     //除了光帆之外的干重
	float MirrorMass;                  //光帆质量 
	float KBLG;                        //反射率 
	float ShipWeave;                   //角秒：一千米尺度的物体来回振幅为2.4毫米  
	float DensityofMirror;             //
}; 

struct ShipsDefine            //描述飞船物理量 
{
	unsigned int IdentifyNum;          //
	unsigned int NumofShips;           //舰队舰船数量 
	unsigned int SystemFrom;           //来自哪一个恒星系 
	unsigned int SystemTo;             //前往哪一个恒星系
	float ProgessPershent;             //进度条(百分比） 
	double PersentofC;                 //当前速度
	float LaunchTime;                  //发射时间 
	float TargetTime;                  //抵达时间  
};

//函数部分 
//恒星参数计算函数 
//已验证 
float StarsTemp(struct StarsSelf SSCP); 
float Starslum(double mass); 
float StarsRadius(double mass);
//光帆飞船计算函数 
//有待检验 
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


#pragma message("CLY-Version-0.2.1")  ///在编译器中显示版本号
