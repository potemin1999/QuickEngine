/*
 *  Only for personal usage
 */

#include "Camera.h"

using namespace QEGraphics;
/* 
 * FLOAT SHIFT IN ARRAY;
 */




/*
 * uses major column matrix :
 * [ 0 , 4 , 8 , 12]
 * [ 1 , 5 , 9 , 13]
 * [ 2 , 6 , 10, 14]
 * [ 3 , 7 , 11, 15]
 */
 
/*
 * mPos    - camera position
 * mView   - normalized look vector, autocomputed
 * mUp     - up vector
 * mStrafe -  (mView * mUp) , right move vector, autocomputed
 */

/*float mPos[3];  //Camera position;
float mView[3]; //Camera look at this;
float mUp[3];   //Points on top;
float mStrafe[3];
*/


//float mRotation[3]; //0 +right -left  //1 +up -down
	

/*
 * Matrix which are used to create image
 */
 

/*
 *constants and other variables
 */





int FSIA = 1;
float piD2= (float)(3.14159f/2.0f);
float sTemp[32];
float average_look_time = 0;



long look_count=1;
	






	
Camera::Camera(){
	mRotation[0] = 0;
	mRotation[1] = 0;
	mRotation[2] = 0;
	mUp = vec3(0, 1, 0);
}



void Camera::update_perspective_matrix() {
	set_identity(&mProjectionMatrix);
	mProjectionMatrix = perspective( fovy, aspect, nearZ, farZ);

	log("perspective matrix updated aspect %f  fovy %f  distance %f\n", aspect, fovy, farZ);
}

void Camera::set_perspective(float aspect, float fovy, float distance) {
	Camera::aspect = aspect;
	Camera::fovy = fovy;
	Camera::farZ = distance;
	Camera::nearZ = 0.01f;
	update_perspective_matrix();
}

void Camera::set_aspect(float aspect) {
	Camera::aspect = aspect;
	update_perspective_matrix();
}





void Camera::position_camera(vec3* pos, vec3* view, vec3* up) {
	mPos.x = pos->x;
	mPos.y = pos->y;
	mPos.z = pos->z;
	mView.x = view->x - pos->x;
	mView.x = view->y - pos->y;
	mView.z = view->z - pos->z;
	mUp.x = up->x;
	mUp.y = up->y;
	mUp.z = up->z;
	normalize(&mView);
	log("set  position x:%f y:%f z:%f\n", mPos.x, mPos.y, mPos.z);
	log("set   look at x:%f y:%f z:%f\n", mView.x,mView.y,mView.z);
	log("set up vector x:%f y:%f z:%f\n", mUp.x,mUp.y,mUp.z);
}



float Camera::sqrt_inv(float x){
	float half = x*0.5f;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - half*x*x);
	return x;
}


	
	/*
	 *  length of v gets to 1.0f
	 */


void Camera::normalize(vec3* v){
	float m = sqrt_inv(v->length());
	v->x = v->x * m;
	v->y = v->y * m;
	v->z = v->z * m;
}





	


void Camera::position_camera(vec3* pos){
	mPos.x = pos->x;
	mPos.y = pos->y;
	mPos.z = pos->z;
}



void Camera::rotate_view_right(float angle){
	vec3* v = new vec3[3];
	v->x = mView.x ;//- mPos[0];
	v->y = mView.y ;//- mPos[1];
	v->z = mView.z ;//- mPos[2];
	mView.x = (float)(cos(-angle) * v->x - sin(-angle) * v->z);
	mView.z = (float)(sin(-angle) * v->x + cos(-angle) * v->z);
	mRotation[0] += angle;
}
	
	

void Camera::rotate_view_up(float angle){
	mRotation[1] += angle;
	mView.y = mRotation[1];
}


void Camera::move_camera(float distance){
	vec3 v;
	v.x = mView.x;
	v.y = mView.y;
	v.z = mView.z;
	normalize(&v);
	mPos.x = mPos.x + v.x * distance;
	mPos.z = mPos.z + v.z * distance;
}
	
void Camera::move_camera_up(float distance){
	mPos += mUp * distance;
}
	
void Camera::move_strafe(float distance){
	update_strafe();
	mPos += mStrafe * distance;
	//mPos.x = mPos.x + mStrafe.x * distance;
	//mPos.z = mPos.z + mStrafe.z * distance;
}
	
void Camera::update_strafe(){
	mStrafe = cross(mView, mUp);
	//cross(mStrafe,mView,mUp);
	normalize(&mStrafe);
}


void Camera::look(){
	//set_identity(mProjectionMatrix, 0);
	//set_identity(mModelMatrix, 0);
	//set_identity(mViewMatrix, 0);
	//perspective(mProjectionMatrix, 0, fovy, aspect, nearZ, farZ);
	compute_look(&mViewMatrix);
	
	//mMVPMatrix = mViewMatrix * mProjectionMatrix;
	mVMatrix;
	mVPMatrix = mProjectionMatrix *  mViewMatrix;// *mViewMatrix;
	//multiply(mMVMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
	//multiply(mMVMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
	//multiply(mMVPMatrix, 0, mProjectionMatrix, 0, mMVMatrix, 0);
	//multiply(mMVPMatrix, 0, mProjectionMatrix, 0, mMVMatrix, 0);
	//multiply(mMVPMatrix, 0, mViewMatrix, 0, mProjectionMatrix, 0);
}
	

/*
	
void Camera::perspective(float* m, int o, float fovy, float aspect, float zN, float zF) {
	float t = tan(fovy);// *(3.14159 / 180));
	float f = 1.0f / t;
	float r = 1.0f / (zN - zF);
	set_identity(m, o);
	int i = f;
	//for (int i = 0; i < 16; i++)m[o + i] = 0;
	m[o + 0] = f / aspect;
	m[o + 1] = 0.0f;
	m[o + 2] = 0.0f;
	m[o + 3] = 0.0f;
	m[o + 4] = 0.0f;
	m[o + 5] = -f;
	m[o + 6] = 0.0f;
	m[o + 7] = 0.0f;
	m[o + 8] = 0.0f;
	m[o + 9] = 0.0f;
	m[o + 10] = (zF + zN) * r;
	m[o + 11] = -1.0f;
	m[o + 12] = 0.0f;
	m[o + 13] = 0.0f;
	m[o + 14] = 2.0f * zF * zN * r;
	m[o + 15] = 0.0f;
	
	//m[5 + o] = -1.0f;
	//m[o + 0] = 0.8568f;
	//m[o + 5] = 1.4281f;
	//m[o + 10] = 1.00033f;
	//m[o + 11] = -1.0f;
	//m[o + 11] = -1.142f;
	//m[o + 14] = -1.0f;
}
	
*/
void Camera::set_identity(mat4* f){
	//(&f)[0][0] = 1.0f;
	
}

	/*
void Camera::translate(float* m,int o,float* v){
	translate(m,o,v[0],v[1],v[2]);
}
	
void Camera::translate(float* m,int o,float x, float y, float z) {
	for (int i=0 ; i<4 ; i++) {
		int mi = o + i;
		m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
	}
}
	



*/	
void Camera::compute_look(mat4* m){
	if (mRotation[1] >= piD2) mRotation[1] = piD2;
	if (mRotation[1] <= -piD2) mRotation[1] = -piD2;
	float horAR = mRotation[0];
	float vertAR = mRotation[1];
	float zAR = mRotation[2];
	//vec3 dir = vec3(cos(vertAR)*sin(horAR), cos(vertAR)*cos(horAR), sin(vertAR));
	vec3 dir = vec3(cos(vertAR)*sin(horAR),sin(vertAR), cos(vertAR)*cos(horAR));
	//mViewMatrix += vec4(mPos, 1.0);
	//set_identity(&mViewMatrix);
	//*m = translate(mProjectionMatrix, mPos);
	//*m = rotate(*m, horAR, mUp);
	//mViewMatrix = lookAt(mPos, mView, mUp);
	//mViewMatrix = glm::rotate(mViewMatrix, horAR, mUp);
	//mViewMatrix = glm::translate(mViewMatrix, mPos);
	mViewMatrix = lookAt(mPos, mPos + dir, mUp);
	mVMatrix = lookAt(vec3(0.0), dir, mUp);
	/*
	float x = mRotation[1];
	float z = mRotation[2];
	float cx = cos(x); 
	float sx = sin(x);
	float cz = cos(z); 
	float sz = sin(z);
		
	m[o + 0] =  cz; 
	m[o + 1] = -sz;
	m[o + 2] = 0.0f;
	m[o + 3] = 0.0f;
	m[o + 4] = cx * sz;
	m[o + 5] = cx * cz;
	m[o + 6] = -sx;
	m[o + 7] = 0.0f;
	m[o + 8] = sx * sz;
	m[o + 9] = sx * cz;
	m[o +10] = cx ;
	m[o +11] = 0.0f;
	m[o +12] = 0.0f;
	m[o +13] = 0.0f;
	m[o +14] = 0.0f;
	m[o +15] = 1.0f;
		
	float a = piD2 + mRotation[0];
	float s = sin(a);
	float c = cos(a);
		
		
	sTemp[15] = 0;
	sTemp[16] = c;
	sTemp[17] = s;
	sTemp[18] = 0;
	sTemp[19] = 0;
	sTemp[20] =-s;
	sTemp[21] = c;
	sTemp[22] = 0;
	sTemp[23] = 0;
	sTemp[24] = 0;
	sTemp[25] = 0;
	sTemp[26] = 1;
	sTemp[27] = 0;
	sTemp[28] = 0;
	sTemp[29] = 0;
	sTemp[30] = 0;
	sTemp[31] = 1;
		
	sTemp[ 0] = (m[0 + o] * sTemp[16]) + (m[4 + o] * sTemp[17]);
	sTemp[ 1] = (m[1 + o] * sTemp[16]) + (m[5 + o] * sTemp[17]);
	sTemp[ 2] = m[6 + o] * sTemp[17];
	sTemp[ 4] = (m[0 + o] * sTemp[20]) + (m[4 + o] * sTemp[21]);
	sTemp[ 5] = (m[1 + o] * sTemp[20]) + (m[5 + o] * sTemp[21]);
	sTemp[ 6] = m[6 + o] * sTemp[21];
	sTemp[ 8] = m[8 + o];
	sTemp[ 9] = m[9 + o];
	sTemp[10] = m[10+ o];
	sTemp[15] = m[15+ o];
	
	m[ 0] = sTemp[ 0];
	m[ 1] = sTemp[ 1];
	m[ 2] = sTemp[ 2];
	m[ 4] = sTemp[ 4];
	m[ 5] = sTemp[ 5];
	m[ 6] = sTemp[ 6];
	m[ 8] = sTemp[ 8];
	m[ 9] = sTemp[ 9];
	m[10] = sTemp[10];
	m[15] = sTemp[15];
		
		
	m[12+o] += m[o+0] * -mPos[0] + m[4 + o] * -mPos[1] + m[8 + o] * -mPos[2];
	m[13+o] += m[o+1] * -mPos[0] + m[5 + o] * -mPos[1] + m[9 + o] * -mPos[2];
	m[14+o] += m[o+2] * -mPos[0] + m[6 + o] * -mPos[1] + m[10+ o] * -mPos[2];
	*/
}
	
