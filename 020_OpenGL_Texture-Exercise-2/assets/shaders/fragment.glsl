#version 460 core 

out vec4 FragColor; // 输出片元颜色的变量，用于渲染到屏幕的最终颜色
in vec3 color;      // 输入的颜色信息（来自顶点着色器传递过来的颜色数据）
in vec2 uv;         // 输入的纹理坐标，用于采样纹理

uniform sampler2D grassSampler; // 草地纹理的采样器，指向一个 2D 纹理
uniform sampler2D landSampler;  // 土地纹理的采样器，指向一个 2D 纹理
uniform sampler2D noiseSampler; // 噪声采样器


void main()
{
    // 使用纹理坐标 `uv` 从草地纹理中获取颜色
    vec4 grassColor = texture(grassSampler, uv); 
    // 使用纹理坐标 `uv` 从土地纹理中获取颜色
    vec4 landColor = texture(landSampler, uv);
    // 使用纹理坐标 `uv` 从噪声纹理中获取颜色
    vec4 noiseColor = texture(noiseSampler, uv);

    // 将草地颜色和土地颜色进行混合，权重各自为 0.5，得到最终的颜色
    //vec4 finalColor = grassColor * 0.5 + landColor * 0.5;
    //vec4 finalColor = grassColor * weight + landColor * (1-weight);//手动编写混合函数
    float weight=noiseColor.r;//黑白图片，每个像素的r  g   b都相等
    vec4 finalColor = mix(grassColor,landColor,weight);//GLSL语言，内置混合函数
    // 设置最终的片元颜色，只保留 RGB 颜色部分，alpha 设置为 1.0（完全不透明）
    FragColor = vec4(finalColor.rgb, 1.0);
}
