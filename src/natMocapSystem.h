#pragma once
//Cortex�w�b�_�̓ǂݍ���
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
	float **getTemplateMarkerPosPointer(int _tempId);		//ID�ɑΉ�����e���v���[�g�ʒu�z��|�C���^��Ԃ�
	float **getUnidentifiedMarkerPosPointer();				//Unidentified�}�[�J�[�ʒu�z��|�C���^��Ԃ�
	int getUnidentifiedMarkerNum();						//Unidentified�}�[�J�[�̐���Ԃ�
	int getTemplateNum();								//�e���v���[�g�̐���Ԃ�
	std::string getTemplateName(int _tempId);			//�e���v���[�g�̖��O��Ԃ�
	int getTemplateMarkerNum(int _tempId);
private:
	bool isCortexOk;	//Cortex�ƘA�g�ł�����
	unsigned int tempNum;	//�e���v���[�g�̐�
	unsigned int unidentifiedMarkerNum;	//Unidentified�}�[�J�̐�
	/***** Cortex���瑗���Ă���f�[�^�i�[�ϐ� *****/
	sFrameOfData* frameDataFromCortex;	//�P�t���[���̃f�[�^���ׂ�
	tMarkerData* markerDataFromCortex;	//�}�[�J�Q�̈ʒu�f�[�^
	/***** �}�[�J�[�̈ʒu�ۑ��z�� *****/
	float ***templateMarkerPos;		//templateMarkerPos[�e���v���[�g��][�}�[�J�[��][xyz]�̓��I�R�����z��
	float **unidentifiedMarkerPos;		//undefinedMarkerPos[�ő�}�[�J�[��][xyz]�̂Q�����z��
	std::string *templateNames;		//�e���v���[�g�̖��O��ۑ����铮�I�z��
	int *templateMarkerNum;			//�e�e���v���[�g�̃}�[�J���z��
};

};