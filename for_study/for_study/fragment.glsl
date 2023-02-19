#version 330
//out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//FragColor: ����� ������ ������ ������ ���۷� ���� ��. 

//���ؽ� ���̴����Լ� ���� ����
in vec3 out_Normal;
in vec3 FragPos;
in vec2 out_Texture;

//���� ���
out vec4 FragColor;

uniform vec3 objectColor;                                                   //--- ���� ���α׷����� ������ ��ü�� ���� 
uniform vec3 lightColor;													//--- ���� ���α׷����� ������ ���� ����
uniform vec3 lightPos;
uniform vec3 cameraEye;
uniform float ambientLight;
uniform sampler2D outTexture;

void main(void) 
{
	vec3 Normal = out_Normal;

	//�ں��Ʈ (�ֺ�����(��������))
	//float ambientLight = 0.5;                                             //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0 
	vec3 ambient = ambientLight * lightColor;							    //--- �ֺ� ����

	vec3 distence = (lightPos - FragPos); // �Ÿ�
	float length = sqrt(distence.x*distence.x + distence.y*distence.y + distence.z*distence.z);

	vec3 length_color = lightColor / (length / 30);


	//��ǻ�� (��� �ݻ� ����)
	vec3 normalVector = normalize(Normal);							// �븻�� ����ȭ

	vec3 lightDir = normalize(lightPos - FragPos);					// ǥ��� ���� ��ġ�� ���� ���� ����	
	
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);		// N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.) 
	//vec3 diffuse = diffuseLight * lightColor;						// ����ݻ����� = ����ݻ簪 * �������
	vec3 diffuse = diffuseLight * (length_color);						// ����ݻ����� = ����ݻ簪 * �������
	
	//����ŧ�� (�ſ� �ݻ� ����)
	int shininess = 64;										// ����(�Ų�����) ���
	vec3 viewDir = normalize(cameraEye - FragPos);					// ������ ���� ����
	vec3 reflectDir = reflect(-lightDir, normalVector);				// �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ��� 
	float specularLight = max(dot(viewDir, reflectDir), 0.0);		// ���� ����� �ݻ� ������ ���� ������ ���� ����
	specularLight = pow(specularLight, shininess);					// shininess ������ ���̶���Ʈ ǥ��
	//vec3 specular = specularLight * lightColor;						// �ſ� �ݻ� ����
	vec3 specular = specularLight * (length_color);

	vec3 result = (ambient + diffuse + specular) * objectColor;		// ��ü�� ���� �ֺ������� ���Ͽ� ���� ��ü ���� ����

	FragColor = vec4 (result , 1.0);

	if(out_Texture.x >= 0){
		FragColor = texture(outTexture, out_Texture)  * FragColor;
	}
}