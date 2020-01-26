
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include "main.h"
#include <time.h>

#define CAMERA_FAR	5.0F
#define CAMERA_NEAR 1.0F

#define WIDTH 1280
#define HEIGHT 720


#define GL_CALL(func)\
    func;\
    {\
        GLenum error;\
        while((error = glGetError()) != GL_NO_ERROR){\
            ASSERT(false, "line:(%d) ERROR: [0x%x]\n", __LINE__, error);\
        }\
    }

float clamp01(float value){
    if(value > 1.0f)
        return 1.0f;
    else if(value < 0.0f)
        return 0.0f;
    return value;
}


void RenderGeometryPass(uint32 vbo, uint32 ibo, Shader& geometryShader, uint32 framebuffer, uint32 count, float rotation) {
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glEnable(GL_CULL_FACE));

    geometryShader.Bind();

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3), (const void*)0));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3), (const void*)(sizeof(float) * 3)));

	GL_CALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void*)0));
}
void RenderLightPass(uint32 vbo, uint32 readFramebuffer, Shader& lightPassShader, uint32* textures, uint32 numVertices) {
	lightPassShader.Bind();
	GL_CALL(glClearColor(0.8f, 0.2f, 0.3f, 1.0f));
	GL_CALL(glDisable(GL_DEPTH_TEST));
    GL_CALL(glDisable(GL_CULL_FACE));

	GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer));
	GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));


	GL_CALL(glBindTextureUnit(0, textures[0]));
	GL_CALL(glBindTextureUnit(1, textures[1]));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0));
	GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices));
}

int main() {

	if (!glfwInit()) {
		std::cout << "Failed To Initialize GLFW!" << std::endl;
		return 0;
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	int STATUS = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!STATUS) {
		std::cout << "Failed To Initialize Glad!" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwSwapInterval(1);


	const uint32 numBuffers = 2;
	uint32_t textures[numBuffers];
	uint32_t depthTexture;
	uint32_t framebuffer;
	GL_CALL(glCreateFramebuffers(1, &framebuffer));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));

    GL_CALL(glCreateTextures(GL_TEXTURE_2D, 2, textures));
	GL_CALL(glCreateRenderbuffers(1, &depthTexture));

	for (int i = 0; i < 2; i++) {
        GL_CALL(glTextureStorage2D(textures[i], 1, GL_RGB32F, WIDTH, HEIGHT));
        GL_CALL(glTextureParameteri(textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL_CALL(glTextureParameteri(textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL_CALL(glTextureParameteri(textures[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CALL(glTextureParameteri(textures[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0));
	}

	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, depthTexture));
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, WIDTH, HEIGHT));
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture));


	GLenum drawBuffers[numBuffers] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0 + 1 };
	GL_CALL(glDrawBuffers(numBuffers, drawBuffers));

	GL_CALL(GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (!Status) {
		std::cout << "Failed To Validate Frambuffer Status" << std::endl;
		glfwTerminate();
	}

	const uint32 numVertices = 6 * 4;

	float verts[numVertices * (3 + 3)] = {
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f
	};

	const uint32_t numIndices = numVertices / 4 * 6;
	uint32_t indices[numIndices];
	for (int i = 0; i < numIndices / 6; i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 0;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
	}


	uint32_t mainIBO, mainVBO;

	GL_CALL(glCreateBuffers(1, &mainVBO);)
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mainVBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW));

	GL_CALL(glCreateBuffers(1, &mainIBO));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainIBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3), (const void*)0));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3), (const void*)(sizeof(float) * 3)));


	Light light;
	light.pos = {0.0f, 0.0f, -1.0f};
	light.radius = 2.5f;

	uint32 quadVBO;

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	float data[4*3] = {
		-10.0f,-10.0f,  -5.0f,
	 	10.0f,-10.0f,  -5.0f,
		10.0f, 10.0f, -5.0f,
		-10.0f, 10.0f, -5.0f
	};
	glBufferData(GL_ARRAY_BUFFER,  4 * 3 * sizeof(float), data, GL_STATIC_DRAW);


	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));


	// Shader Setup
	mat4 projection = PerspectiveTransform(CAMERA_NEAR, CAMERA_FAR, 45.f, WIDTH / (float)HEIGHT);

	Shader lightPassShader("./shaders/light_pass.vs", "./shaders/light_pass.fs");
	lightPassShader.Bind();
	lightPassShader.SetInt("u_posTexture", 0);
	lightPassShader.SetInt("u_normalTexture", 1);
	lightPassShader.SetFloat3("u_lightPos", light.pos.x, light.pos.y, light.pos.z);
	lightPassShader.SetFloat2("u_screenSize", (float)WIDTH, (float)HEIGHT);
	lightPassShader.SetMat4("u_projection", projection.elements);

	Shader geometryPassShader("./shaders/geometry_pass.vs", "./shaders/geometry_pass.fs");
	geometryPassShader.Bind();
	geometryPassShader.SetMat4("u_projection", projection.elements);
	
	auto start = clock();
	float angle = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		GL_CALL(glfwPollEvents());
		long current = clock();
		long elapsed = current - start;
		start = current;
		angle += elapsed * 0.01f;

		geometryPassShader.Bind();
		mat4 model = Translate(0.0f, 0.0f, -2.5f) * RotateY(angle) * RotateX(angle * 3.0f);
		geometryPassShader.SetMat4("u_model", model.elements);

		RenderGeometryPass(mainVBO, mainIBO, geometryPassShader, framebuffer, numIndices, 0.0f);
		RenderLightPass(quadVBO, framebuffer, lightPassShader, textures, numVertices+1);
		GL_CALL(glfwSwapBuffers(window));
	}


    glfwTerminate();

}