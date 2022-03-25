#include <Acrylic.h>

#include "imgui/imgui.h"


class TestLayer : public Acrylic::Layer {
public:
	TestLayer() : Layer("Test"), m_MainCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
	{
		// Triangle

		m_TriangleVertexArray.reset(Acrylic::VertexArray::Create());

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Acrylic::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Acrylic::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

		Acrylic::BufferLayout layout = {
			{ Acrylic::ShaderDataType::Float3, "a_Position" },
			{ Acrylic::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_TriangleVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Acrylic::IndexBuffer> indexBuffer;
		indexBuffer.reset(Acrylic::IndexBuffer::Create(indices, sizeof(triangleVertices) / sizeof(uint32_t)));
		m_TriangleVertexArray->SetIndexBuffer(indexBuffer);

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

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position + 0.5;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		triangleShader.reset(new Acrylic::Shader(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position + 0.5;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
		if (Acrylic::Input::IsKeyPressed(AC_KEY_W))
			m_CameraPosition.y += m_CameraMovementSpeed;
		if (Acrylic::Input::IsKeyPressed(AC_KEY_S))
			m_CameraPosition.y -= m_CameraMovementSpeed;
		if (Acrylic::Input::IsKeyPressed(AC_KEY_A))
			m_CameraPosition.x -= m_CameraMovementSpeed;
		if (Acrylic::Input::IsKeyPressed(AC_KEY_D))
			m_CameraPosition.x += m_CameraMovementSpeed;

		if (Acrylic::Input::IsKeyPressed(AC_KEY_Q)) {
			m_CameraRotation -= m_CameraRotationSpeed;
		}
		if (Acrylic::Input::IsKeyPressed(AC_KEY_E)) {
			m_CameraRotation += m_CameraRotationSpeed;
		}

		m_MainCamera.SetPosition(m_CameraPosition);
		m_MainCamera.SetRotation(m_CameraRotation);

		Acrylic::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Acrylic::RenderCommand::Clear();

		Acrylic::Renderer::BeginScene(m_MainCamera);
		{
			Acrylic::Renderer::Submit(m_SquareVertexArray, squareShader);
			Acrylic::Renderer::Submit(m_TriangleVertexArray, triangleShader);
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
		Acrylic::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Acrylic::KeyPressedEvent>(AC_BIND_EVENT_FN(TestLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Acrylic::KeyPressedEvent& event)
	{
		AC_TRACE("{0}", glm::to_string(m_MainCamera.GetPosition()));
		return false;
	}

private:
	std::shared_ptr<Acrylic::Shader> triangleShader;
	std::shared_ptr<Acrylic::VertexArray> m_TriangleVertexArray;

	std::shared_ptr<Acrylic::Shader> squareShader;
	std::shared_ptr<Acrylic::VertexArray> m_SquareVertexArray;

	Acrylic::OrthographicCamera m_MainCamera;
	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 0.05f;
	float m_CameraRotation;
	float m_CameraRotationSpeed = 1.0f;
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