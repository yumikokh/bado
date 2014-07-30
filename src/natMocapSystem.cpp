#include "natMocapSystem.h"
#include <iostream>
/***** nagayoshi tool (nat) *****/
namespace nat{

MocapSystem::MocapSystem(void)
{
}


MocapSystem::~MocapSystem(void)
{
	/***** �A�g�ɐ��������Ƃ��̂݃f�X�g���N�^���� *****/
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
	/***** Cortex�Ƃ̘A�g������ *****/
	if(RC_Okay == Cortex_Initialize(const_cast<char*>("10.1.1.250"), const_cast<char*>("10.1.1.250"))){
	//if(RC_Okay == Cortex_Initialize(const_cast<char*>("192.168.11.11"), const_cast<char*>("192.168.11.11"))){
	/***** �A�g�ɐ��������ꍇ *****/
		std::cout << "Cortex_Initialize Successed" << std::endl;
		this->isCortexOk = true;
		/*****�@�e���v���[�g�}�[�J�f�[�^�z��̓��I�������m�ۏ����@*****/
		this->frameDataFromCortex = Cortex_GetCurrentFrame();		//���݂̃t���[���f�[�^���擾
		this->tempNum = this->frameDataFromCortex->nBodies;			//�e���v���[�g�̐����擾
		std::cout << "Template Number = " << this->tempNum << std::endl;	//�R���\�[���Ƀe���v���[�g�̐����o��
		this->templateMarkerPos = new float**[this->tempNum];		//�e���v���[�g�̐������������𓮓I�m��
		this->templateNames = new std::string[this->tempNum];		//�e���v���[�g�̐������e���v���[�g���O�z��𓮓I�m��
		this->templateMarkerNum = new int[this->tempNum];			//�e���v���[�g�̐������e�e���v���[�g�̃}�[�J���z��𓮓I�m��
		for(int i=0; i<this->tempNum; i++){
			this->templateNames[i] = this->frameDataFromCortex->BodyData[i].szName;	//�e���v���[�g�̖��O���擾
			std::cout << i << " : " << this->templateNames[i] << std::endl;	//�R���\�[���Ƀe���v���[�g��ID�ԍ��Ɩ��O���o��
			//�e�e���v���[�g�̃}�[�J�[�̐������������𓮓I�m��
			int mNum = this->frameDataFromCortex->BodyData[i].nMarkers;
			this->templateMarkerNum[i] = mNum;
			this->templateMarkerPos[i] = new float*[mNum];
			//����Ƀ}�[�J�[�ЂƂɑ΂��āix,y,z�j3���̃������𓮓I�m��
			for(int j=0; j<mNum; j++){
				this->templateMarkerPos[i][j] = new float[3];
				//�����l�Œl��������
				this->templateMarkerPos[i][j][0] = MS_MARKER_POS_INF;
				this->templateMarkerPos[i][j][1] = MS_MARKER_POS_INF;
				this->templateMarkerPos[i][j][2] = MS_MARKER_POS_INF;
			}
		}
		/***** Unidnetified�}�[�J�[�f�[�^�z��̓��I�������m�� *****/
		this->unidentifiedMarkerPos = new float*[MS_UNIDENTIFIED_MARKER_MAX_NUM];
		for(int i=0; i<MS_UNIDENTIFIED_MARKER_MAX_NUM; i++){
			this->unidentifiedMarkerPos[i] = new float[3];
			this->unidentifiedMarkerPos[i][0] = MS_MARKER_POS_INF;
			this->unidentifiedMarkerPos[i][1] = MS_MARKER_POS_INF;
			this->unidentifiedMarkerPos[i][2] = MS_MARKER_POS_INF;
		}
	}else{
	/***** �A�g�Ɏ��s�����ꍇ *****/
		std::cout << "Cortex_Initialize Failed" << std::endl;
		isCortexOk = false;
	}
}

void MocapSystem::update(void)
{
	/***** �A�g�ɐ��������Ƃ��̂ݍX�V *****/
	if(isCortexOk){
		this->frameDataFromCortex = Cortex_GetCurrentFrame();	//���݂̃t���[���f�[�^���擾
		/***** �e���v���[�g�}�[�J�[�f�[�^�̍X�V *****/
		for(int i=0; i<this->tempNum; i++){
			int mNum = this->frameDataFromCortex->BodyData[i].nMarkers;		//i�Ԗڂ̃e���v���[�g�̃}�[�J�̐����擾
			this->markerDataFromCortex=this->frameDataFromCortex->BodyData[i].Markers;		//i�Ԗڂ̃e���v���[�g�}�[�J�f�[�^���擾
			//i�Ԗڂ̃e���v���[�g�}�[�J�ʒu�������̃}�[�J�z��֊i�[
			for(int j=0; j<mNum; j++){
				if(this->markerDataFromCortex[j][0] > 1000000){
					//�e���v���[�g���O�ꂽ�Ƃ��i�ɒ[�ɑ傫���l�������Ƃ��j�X�V���Ȃ�
				}else{
					this->templateMarkerPos[i][j][0] = this->markerDataFromCortex[j][0];
					this->templateMarkerPos[i][j][1] = this->markerDataFromCortex[j][1];
					this->templateMarkerPos[i][j][2] = this->markerDataFromCortex[j][2];
				}
			}
		}
		/***** Undefined�}�[�J�[�f�[�^�̍X�V *****/
		this->unidentifiedMarkerNum = this->frameDataFromCortex->nUnidentifiedMarkers;		//undefined�}�[�J�[�̐����擾
		//Undefined�}�[�J�[�̍ő���𒴂��Ă��܂����ꍇ�A�ő���Ɋۂ߂�
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