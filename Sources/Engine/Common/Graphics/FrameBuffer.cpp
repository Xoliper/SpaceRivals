/*
 * Framebuffer.cpp
 *
 *  Created on: 30 lip 2018
 *      Author: Xoliper
 */

#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(int width, int height){
	this->width = width;
	this->height = height;
	glGenFramebuffers(1, &framebufferID);

	AddColorBuffer();	//Add one color buffer
	AddDepthStencilBuffer();
}

FrameBuffer::~FrameBuffer(){
	glDeleteFramebuffers(1, &framebufferID);
}

bool FrameBuffer::ValidStatus(){
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;
	return true;
}

void FrameBuffer::Bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID); //GL_DRAW_FRAMEBUFFER, GL_READFRAMEBUFFER
}

void FrameBuffer::Unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AddColorBuffer(){

	if(ColorBuffersAmount() > 15){
		throw(Exception("FrameBuffer", "AddColorBuffer", "Exceeded maximum color buffers amount!"));
	}

	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(
	    GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texColorAttachments.push_back(GL_COLOR_ATTACHMENT0+texColorBuffers.size());
	texColorBuffers.push_back(texColorBuffer);

	//SetRenderColorBuffer(0);
}

void FrameBuffer::SetRenderColorBuffer(int bufferID){
	glFramebufferTexture2D(
	    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+bufferID, GL_TEXTURE_2D, texColorBuffers[bufferID], 0
	);

	/*glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel( GL_SMOOTH);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

}

void FrameBuffer::SetRenderDepthStencilBuffer(){
	glFramebufferRenderbuffer(
	    GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, depthStencilBuffer
	);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
}

void FrameBuffer::AddDepthStencilBuffer(){
	glGenRenderbuffers(1, &depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	 //glDrawBuffer(GL_NONE);
}

size_t FrameBuffer::ColorBuffersAmount(){
	return texColorBuffers.size();
}








FrameBufferManager::FrameBufferManager(){
}

FrameBufferManager::~FrameBufferManager(){
	for(size_t i = 0; i<fbs.size(); i++){
		delete fbs[i];
	}
}

size_t FrameBufferManager::GetBuffersAmount(){
	return fbs.size();
}

void FrameBufferManager::SetDefaultFB(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferManager::SetActiveColorBuffer(int id){
	fbs[id]->Bind();
	fbs[id]->SetRenderColorBuffer(0);
	SetActiveDepthStencilBuffer(0);
}

void FrameBufferManager::SetActiveDepthStencilBuffer(int id){
	fbs[id]->SetRenderDepthStencilBuffer();
}

void FrameBufferManager::ClearBuffers(){
	for(size_t i = 0; i<fbs.size(); i++){
		fbs[i]->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FrameBufferManager::AddBuffer(int width, int height){
	FrameBuffer * tempFB = new FrameBuffer(width, height);
	if(tempFB != NULL && tempFB->ValidStatus() == false) throw(Exception("FrameBufferManager", "AddBuffer", "Error while creating buffer!"));
	fbs.push_back(tempFB);
}

GLuint FrameBufferManager::GetFrameBufferColor(int id){
	return fbs[id]->texColorBuffers[0];
}

GLuint FrameBufferManager::GetDepthStencilTexture(int id){
	return fbs[id]->depthTexture;
}
