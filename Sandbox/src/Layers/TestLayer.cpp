#include "TestLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


TestLayer::TestLayer() : Layer("Test"), m_MainCameraController(1280.0f / 720.0f, true)
{
	// Triangle
	m_TriangleVertexArray = Acrylic::VertexArray::Create();

	float triangleVertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	};

	Acrylic::Ref<Acrylic::VertexBuffer> vertexBuffer = Acrylic::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));

	Acrylic::BufferLayout layout = {
		{ Acrylic::ShaderDataType::Float3, "a_Position" },
		{ Acrylic::ShaderDataType::Float4, "a_Color" }
	};

	vertexBuffer->SetLayout(layout);
	m_TriangleVertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Acrylic::Ref<Acrylic::IndexBuffer> indexBuffer = Acrylic::IndexBuffer::Create(indices, sizeof(triangleVertices) / sizeof(uint32_t));
	m_TriangleVertexArray->SetIndexBuffer(indexBuffer);


	// Square
	m_SquareVertexArray = Acrylic::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};


	Acrylic::Ref<Acrylic::VertexBuffer> squareVertexBuffer = Acrylic::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Acrylic::BufferLayout squareLayout = {
		{ Acrylic::ShaderDataType::Float3, "a_Position" },
		{ Acrylic::ShaderDataType::Float2, "a_TextCoord" },
	};

	squareVertexBuffer->SetLayout(squareLayout);
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	uint32_t squareIndices[6] = { 0, 3, 1, 1, 3, 2 };
	Acrylic::Ref<Acrylic::IndexBuffer> squareIndexBuffer = Acrylic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

	std::string vertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position + 0.5;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

	m_TriangleShader = Acrylic::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position + 0.5;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	m_FlatColorShader = Acrylic::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Acrylic::Texture2D::Create("assets/textures/Checkerboard.png");
	m_TransparentTexture = Acrylic::Texture2D::Create("assets/textures/moon.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void TestLayer::OnAttach()
{
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(Acrylic::Timestep ts)
{
	//AC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		// Update
	m_MainCameraController.OnUpdate(ts);

	// Render
	Acrylic::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	Acrylic::RenderCommand::Clear();

	Acrylic::Renderer::BeginScene(m_MainCameraController.GetCamera());
	{
		static glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Acrylic::Renderer::Submit(m_SquareVertexArray, m_FlatColorShader, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Acrylic::Renderer::Submit(m_SquareVertexArray, textureShader, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));
		m_TransparentTexture->Bind();
		Acrylic::Renderer::Submit(m_SquareVertexArray, textureShader, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));

		// Triangle 
		//Acrylic::Renderer::Submit(m_TriangleVertexArray, m_TriangleShader);
	}
	Acrylic::Renderer::EndScene();
}

void TestLayer::OnEvent(Acrylic::Event& e)
{
	Acrylic::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Acrylic::KeyPressedEvent>(AC_BIND_EVENT_FN(TestLayer::OnKeyPressed));

	m_MainCameraController.OnEvent(e);
}

void TestLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
