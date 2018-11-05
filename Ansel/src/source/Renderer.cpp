#include "../headers/Renderer.h"

namespace Ansel
{
	void Renderer::prepare() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Render(Model model) {
		prepare();

		VAO *vao = model.getVAO();
		vao->bind();

		for (int i = 0; i < 16; i++) {
			if (vao->VBOS.at(i).ID != 0) {
				glEnableVertexAttribArray(i);
			}
		}

		if (vao->bufferExists(Buffer::INDICES)) {
			vao->getBuffer(Buffer::INDICES).bind();

			glDrawElements(GL_LINE_STRIP, model.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 3);

		for (int i = 0; i < 16; i++) {
			if (vao->VBOS.at(i).ID != 0) {
				glDisableVertexAttribArray(i);
			}
		}

		vao->unbind();
	}
}