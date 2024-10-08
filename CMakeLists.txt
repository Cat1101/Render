cmake_minimum_required(VERSION 3.25)
project(Render)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS -m32)

set(DX_DIR_INC "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0")
set(DX_DIR_LIB "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0")
set(DirectXTK, "C:/Program Files (x86)/DirectXTK-main/Inc")


add_executable(Render WIN32 main.cpp
        WIC/WICTextureLoader11.cpp
        WIC/stb_image.h
        D3D.cpp
        D3D.h
        Render/Inc/ShaderProgram.h
        Render/Src/ShaderProgram.cpp
        Render/Inc/Texture.h
        Render/Src/Texture.cpp
        Timer.h
        Timer.cpp
        Render/Inc/Camera.h
        Render/Src/Camera.cpp
        Render/Inc/DefaultRenderObject.h
        Render/Src/DefaultRenderObject.cpp
        Render/Inc/ConstantBuffer.h
        Render/Src/ConstantBuffer.cpp
        Render/Inc/DefaultMaterial.h
        Render/Src/DefaulMaterial.cpp
        Render/Inc/ConstantBufferTypes.h
        Game/Inc/FirstPlayerController.h
        Game/Src/FirstPlayerController.cpp
        Input.h
        Input.cpp
        ObjLoader.h
        ObjLoader.cpp Lights/DirectionLight.h Lights/DirectionLight.cpp Lights/PointLight.h Lights/PointLight.cpp Render/Inc/TesselationShaderProgram.h Render/Src/TesselationShaderProgram.cpp Render/Inc/TesselationDefaultLitMaterial.h Render/Src/TesselationDefaultLitMaterial.cpp Skybox.cpp Skybox.h Render/Inc/UnlitDefaultMaterial.h Render/Src/UnlitDefaultMaterial.cpp DDSTextureLoader11.cpp DDSTextureLoader11.h Render/Inc/DDSTexture.h Render/Src/DDSTexture.cpp Render/Inc/DirShadowMapMaterial.h Render/Src/DirShadowMapMaterial.cpp)

target_include_directories(Render PUBLIC ${DirectXTK}/)
target_include_directories(Render PUBLIC ${DX_DIR_INC}/um)
target_include_directories(Render PUBLIC ${DX_DIR_INC}/shared)

target_link_libraries(Render ${DX_DIR_LIB}/um/x86/d3dcompiler.lib)
target_link_libraries(Render ${DX_DIR_LIB}/um/x86/d3d11.lib)
target_link_libraries(Render ${DX_DIR_LIB}/um/x86/dinput8.lib)
