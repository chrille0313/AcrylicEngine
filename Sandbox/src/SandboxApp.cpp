#include <Acrylic.h>

#include "imgui/imgui.h"
#include "Acrylic/Renderer/Renderer.h"


class TestLayer : public Acrylic::Layer {
public:
	TestLayer() : Layer("Test")
	{
		// Triangle

		m_VertexArray.reset(Acrylic::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Acrylic::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Acrylic::VertexBuffer::Create(vertices, sizeof(vertices)));

		Acrylic::BufferLayout layout = {
			{ Acrylic::ShaderDataType::Float3, "a_Position" },
			{ Acrylic::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Acrylic::IndexBuffer> indexBuffer;
		indexBuffer.reset(Acrylic::IndexBuffer::Create(indices, sizeof(vertices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square

		m_SquareVertexArray.reset(Acrylic::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f
		};


		std::shared_ptr<Acrylic::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Acrylic::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Acrylic::BufferLayout squareLayout = {
			{ Acrylic::ShaderDataType::Float3, "a_Position" },
		};

		squareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 3, 1, 1, 3, 2 };
		std::shared_ptr<Acrylic::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Acrylic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position + 0.5;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;			

			void main()
			{
				color = vec4((v_Position + 1) * 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Acrylic::Shader(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position + 0.5;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			void main()
			{
				color = vec4((v_Position + 1) * 0.5, 1.0);
			}
		)";

		squareShader.reset(new Acrylic::Shader(squareVertexSrc, squareFragmentSrc));
	}

	void OnUpdate() override
	{
		if (Acrylic::Input::IsKeyPressed(AC_KEY_SPACE))
			AC_TRACE("Space key is pressed!");


		Acrylic::Renderer::BeginScene();
		{
			squareShader->Bind();
			Acrylic::Renderer::Submit(m_SquareVertexArray);

			m_Shader->Bind();
			Acrylic::Renderer::Submit(m_VertexArray);
		}
		Acrylic::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello Wordl!");
		//ImGui::End();
	}

	void OnEvent(Acrylic::Event& event) override
	{
		if (event.GetEventType() == Acrylic::EventType::KeyPressed) {
			Acrylic::KeyPressedEvent& e = (Acrylic::KeyPressedEvent&)event;
			AC_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:
	std::shared_ptr<Acrylic::Shader> m_Shader;
	std::shared_ptr<Acrylic::VertexArray> m_VertexArray;

	std::shared_ptr<Acrylic::Shader> squareShader;
	std::shared_ptr<Acrylic::VertexArray> m_SquareVertexArray;
};


class Sandbox : public Acrylic::Application {
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox() {}
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}