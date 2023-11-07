#pragma once
#include <map>
#include <string>
#include <wrl.h>

#include "Texture.h"

/// <summary>
/// �e�N�X�`���}�l�[�W���[
/// </summary>
class TextureManager
{
private:  // �R���X�g���N�^��

	// �V���O���g���p�^�[���̐ݒ�
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;

public: // �ÓI�ȃ����o�֐�

	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// �e�N�X�`�����[�h�֐�(�f�B���N�g���w��Ȃ�)
	/// </summary>
	/// <param name="fileName">Resources���̃t�@�C����</param>
	/// <returns>�e�N�X�`��</returns>
	static Texture* Load(std::string fileName);

	/// <summary>
	/// �e�N�X�`�����[�h�֐�
	/// </summary>
	/// <param name="directoryPath">�e�N�X�`���܂ł̃f�B���N�g���p�X</param>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�e�N�X�`��</returns>
	static Texture* Load(std::string directoryPath, std::string fileName);

public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize();

private: // �ǂݍ��݊֐��Q

	/// <summary>
	/// �e�N�X�`�����[�h�֐�(�f�B���N�g���w��Ȃ�)
	/// </summary>
	/// <param name="fileName">Resources���̃t�@�C����</param>
	/// <returns>�e�N�X�`��</returns>
	Texture* LoadTexture(std::string fileName);

	/// <summary>
	/// �e�N�X�`�����[�h�֐�
	/// </summary>
	/// <param name="directoryPath">�e�N�X�`���܂ł̃f�B���N�g���p�X</param>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�e�N�X�`��</returns>
	Texture* LoadTexture(std::string directoryPath, std::string fileName);

private: // �����o�ϐ�

	// DirectX12�ėp�N���X
	DirectXCommon* dxCommon_;

	// �e�N�X�`���R���e�i
	std::map<std::string, Texture*> textures_;

};

