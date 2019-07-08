/*
 * Framebuffer.hpp
 *
 *  Created on: 30 lip 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_GRAPHICS_FRAMEBUFFER_HPP_
#define ENGINE_COMMON_GRAPHICS_FRAMEBUFFER_HPP_

#include <GL/glew.h>
#include <vector>
#include "../Exceptions.hpp"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void Bind();
	void Unbind();
	bool ValidStatus();

	size_t ColorBuffersAmount();
	void AddColorBuffer();
	void SetRenderColorBuffer(int bufferID);

	void AddDepthStencilBuffer();
	void SetRenderDepthStencilBuffer();

	std::vector<GLuint> texColorBuffers;
	std::vector<GLenum> texColorAttachments;
	GLuint depthTexture;


private:
	GLuint framebufferID;
	GLuint depthStencilBuffer;

	int width;
	int height;

};


class FrameBufferManager {
public:
	FrameBufferManager();
	~FrameBufferManager();

	size_t GetBuffersAmount();
	void SetDefaultFB();
	void SetActiveColorBuffer(int id);
	void SetActiveDepthStencilBuffer(int id);
	void AddBuffer(int width, int height);
	void ClearBuffers();
	GLuint GetFrameBufferColor(int id);
	GLuint GetDepthStencilTexture(int id);

	std::vector<FrameBuffer*> fbs;

};

#endif /* ENGINE_COMMON_GRAPHICS_FRAMEBUFFER_HPP_ */
