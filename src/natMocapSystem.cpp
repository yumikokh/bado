#include "natMocapSystem.h"
#include <iostream>
/***** nagayoshi tool (nat) *****/
namespace nat{

MocapSystem::MocapSystem(void)
{
}


MocapSystem::~MocapSystem(void)
{
	/***** 連携に成功したときのみデストラクタ処理 *****/
	if(isCortexOk){
		for(int i=0; i<this->tempNum; i++){
			int mNum = this->frameDataFromCortex->BodyData[i].nMarkers;
			for(int j=0; j<mNum; j++){
				delete[] this->templateMarkerPos[i][j];
			}
			delete[] this->templateMarkerPos[i];
		}
		delete[] this->templateMarkerPos;
	}
}

void MocapSystem::setup(void)
{
	/***** Cortexとの連携初期化 *****/
	if(RC_Okay == Cortex_Initialize(const_cast<char*>("10.1.1.250"), const_cast<char*>("10.1.1.250"))){
	//if(RC_Okay == Cortex_Initialize(const_cast<char*>("192.168.11.11"), const_cast<char*>("192.168.11.11"))){
	/***** 連携に成功した場合 *****/
		std::cout << "Cortex_Initialize Successed" << std::endl;
		this->isCortexOk = true;
		/*****　テンプレートマーカデータ配列の動的メモリ確保処理　*****/
		this->frameDataFromCortex = Cortex_GetCurrentFrame();		//現在のフレームデータを取得
		this->tempNum = this->frameDataFromCortex->nBodies;			//テンプレートの数を取得
		std::cout << "Template Number = " << this->tempNum << std::endl;	//コンソールにテンプレートの数を出力
		this->templateMarkerPos = new float**[this->tempNum];		//テンプレートの数だけメモリを動的確保
		this->templateNames = new std::string[this->tempNum];		//テンプレートの数だけテンプレート名前配列を動的確保
		this->templateMarkerNum = new int[this->tempNum];			//テンプレートの数だけ各テンプレートのマーカ数配列を動的確保
		for(int i=0; i<this->tempNum; i++){
			this->templateNames[i] = this->frameDataFromCortex->BodyData[i].szName;	//テンプレートの名前を取得
			std::cout << i << " : " << this->templateNames[i] << std::endl;	//コンソールにテンプレートのID番号と名前を出力
			//各テンプレートのマーカーの数だけメモリを動的確保
			int mNum = this->frameDataFromCortex->BodyData[i].nMarkers;
			this->templateMarkerNum[i] = mNum;
			this->templateMarkerPos[i] = new float*[mNum];
			//さらにマーカーひとつに対して（x,y,z）3つ分のメモリを動的確保
			for(int j=0; j<mNum; j++){
				this->templateMarkerPos[i][j] = new float[3];
				//無限値で値を初期化
				this->templateMarkerPos[i][j][0] = MS_MARKER_POS_INF;
				this->templateMarkerPos[i][j][1] = MS_MARKER_POS_INF;
				this->templateMarkerPos[i][j][2] = MS_MARKER_POS_INF;
			}
		}
		/***** Unidnetifiedマーカーデータ配列の動的メモリ確保 *****/
		this->unidentifiedMarkerPos = new float*[MS_UNIDENTIFIED_MARKER_MAX_NUM];
		for(int i=0; i<MS_UNIDENTIFIED_MARKER_MAX_NUM; i++){
			this->unidentifiedMarkerPos[i] = new float[3];
			this->unidentifiedMarkerPos[i][0] = MS_MARKER_POS_INF;
			this->unidentifiedMarkerPos[i][1] = MS_MARKER_POS_INF;
			this->unidentifiedMarkerPos[i][2] = MS_MARKER_POS_INF;
		}
	}else{
	/***** 連携に失敗した場合 *****/
		std::cout << "Cortex_Initialize Failed" << std::endl;
		isCortexOk = false;
	}
}

void MocapSystem::update(void)
{
	/***** 連携に成功したときのみ更新 *****/
	if(isCortexOk){
		this->frameDataFromCortex = Cortex_GetCurrentFrame();	//現在のフレームデータを取得
		/***** テンプレートマーカーデータの更新 *****/
		for(int i=0; i<this->tempNum; i++){
			int mNum = this->frameDataFromCortex->BodyData[i].nMarkers;		//i番目のテンプレートのマーカの数を取得
			this->markerDataFromCortex=this->frameDataFromCortex->BodyData[i].Markers;		//i番目のテンプレートマーカデータを取得
			//i番目のテンプレートマーカ位置を自分のマーカ配列へ格納
			for(int j=0; j<mNum; j++){
				if(this->markerDataFromCortex[j][0] > 1000000){
					//テンプレートが外れたとき（極端に大きい値が来たとき）更新しない
				}else{
					this->templateMarkerPos[i][j][0] = this->markerDataFromCortex[j][0];
					this->templateMarkerPos[i][j][1] = this->markerDataFromCortex[j][1];
					this->templateMarkerPos[i][j][2] = this->markerDataFromCortex[j][2];
				}
			}
		}
		/***** Undefinedマーカーデータの更新 *****/
		this->unidentifiedMarkerNum = this->frameDataFromCortex->nUnidentifiedMarkers;		//undefinedマーカーの数を取得
		//Undefinedマーカーの最大個数を超えてしまった場合、最大個数に丸める
		if(this->unidentifiedMarkerNum > MS_UNIDENTIFIED_MARKER_MAX_NUM){
			this->unidentifiedMarkerNum = MS_UNIDENTIFIED_MARKER_MAX_NUM;
		}
		for(int i=0; i<unidentifiedMarkerNum; i++){
			this->unidentifiedMarkerPos[i][0] = this->frameDataFromCortex->UnidentifiedMarkers[i][0];
			this->unidentifiedMarkerPos[i][1] = this->frameDataFromCortex->UnidentifiedMarkers[i][1];
			this->unidentifiedMarkerPos[i][2] = this->frameDataFromCortex->UnidentifiedMarkers[i][2];
		}
	}
}

float** MocapSystem::getTemplateMarkerPosPointer(int _tempId)
{
	return this->templateMarkerPos[_tempId];
}

float** MocapSystem::getUnidentifiedMarkerPosPointer()
{
	return this->unidentifiedMarkerPos;
}

int MocapSystem::getUnidentifiedMarkerNum()
{
	return this->unidentifiedMarkerNum;
}

int MocapSystem::getTemplateNum()
{
	return this->tempNum;
}

std::string MocapSystem::getTemplateName(int _tempId)
{
	return this->templateNames[_tempId];
}

int MocapSystem::getTemplateMarkerNum(int _tempId)
{
	return this->templateMarkerNum[_tempId];
}

};