#pragma once
//Cortexヘッダの読み込み
#include <string>
#include "Cortex.h"
#define MS_UNIDENTIFIED_MARKER_MAX_NUM 20
#define MS_MARKER_POS_INF 333333
/***** nagayoshi tool (nat) *****/
namespace nat{

class MocapSystem
{
public:
	MocapSystem(void);
	~MocapSystem(void);
	void setup(void);
	void update(void);
	float **getTemplateMarkerPosPointer(int _tempId);		//IDに対応するテンプレート位置配列ポインタを返す
	float **getUnidentifiedMarkerPosPointer();				//Unidentifiedマーカー位置配列ポインタを返す
	int getUnidentifiedMarkerNum();						//Unidentifiedマーカーの数を返す
	int getTemplateNum();								//テンプレートの数を返す
	std::string getTemplateName(int _tempId);			//テンプレートの名前を返す
	int getTemplateMarkerNum(int _tempId);
private:
	bool isCortexOk;	//Cortexと連携できたか
	unsigned int tempNum;	//テンプレートの数
	unsigned int unidentifiedMarkerNum;	//Unidentifiedマーカの数
	/***** Cortexから送られてくるデータ格納変数 *****/
	sFrameOfData* frameDataFromCortex;	//１フレームのデータすべて
	tMarkerData* markerDataFromCortex;	//マーカ群の位置データ
	/***** マーカーの位置保存配列 *****/
	float ***templateMarkerPos;		//templateMarkerPos[テンプレート数][マーカー数][xyz]の動的３次元配列
	float **unidentifiedMarkerPos;		//undefinedMarkerPos[最大マーカー数][xyz]の２次元配列
	std::string *templateNames;		//テンプレートの名前を保存する動的配列
	int *templateMarkerNum;			//各テンプレートのマーカ数配列
};

};