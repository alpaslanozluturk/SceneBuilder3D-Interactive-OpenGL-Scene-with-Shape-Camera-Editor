#version 330 core                                 
                                       
out vec4 fragColor;                                  
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D mySampler;
uniform vec4 uColor = vec4(0.5, 0.5, 0.5, 1.0);

void main()                                            
{    
    // Eğer texture koordinatları (0,0) ise dış yüzeydir
    if(TexCoord == vec2(0.0, 0.0)) {
        fragColor = uColor; // Dış yüzey için düz renk
    }
    else {
        fragColor = texture(mySampler, TexCoord); // İç yüzey için texture
    }
}                                                      