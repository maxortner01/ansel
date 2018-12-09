#include "../../headers/rendering/FrameBuffer.h"

#include <iostream>
#include <GL/glew.h>

namespace Ansel
{
	void FrameBuffer::generateFrameBuffer(Window* w) {
		int e = glGetError();

		// Create the Framebuffer
		glGenFramebuffers(1, &ID);
		bind();

		// Allocate empty texture to bind to the buffer
		color = new Texture(w->getWidth(), w->getHeight(), nullptr, COLOR);
		depth = new Texture(w->getWidth(), w->getHeight(), nullptr, DEPTH);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, color->getID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->getID(), 0);

		glBindTexture(GL_TEXTURE_2D, depth->getID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->getID(), 0);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w->getWidth(), w->getHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Error: Framebuffer not complete!" << std::endl;
			std::cout << "Code:  " << status << std::endl;
		}

		unbind();
	}
	
	FrameBuffer::FrameBuffer(Window* w) {
		generateFrameBuffer(w);
	}

	FrameBuffer::~FrameBuffer() {
		delete color;
		delete depth;
	}

	Texture* FrameBuffer::getColorTexture() const {
		return color;
	}

	Texture* FrameBuffer::getDepthTexture() const {
		return depth;
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}

	void FrameBuffer::clear() {
		bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unbind();
	}
}