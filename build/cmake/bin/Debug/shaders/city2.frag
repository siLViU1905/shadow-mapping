#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 ViewPos;
in vec3 FragPos;
in mat3 TBN;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    // Standard PBR inputs
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    
    sampler2D texture_basecolor1;
    sampler2D texture_basecolor2;
    
    sampler2D texture_normal1;
    sampler2D texture_normal2;
    sampler2D texture_normal_camera1;
    
    sampler2D texture_metalness1;
    sampler2D texture_metalness2;
    
    // Traditional inputs
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    
    sampler2D texture_ambient1;
    sampler2D texture_ambient2;
    
    sampler2D texture_shininess1;
    
    // Light contribution maps
    sampler2D texture_lightmap1;
    sampler2D texture_ao1;           // Ambient occlusion
    sampler2D texture_emissive1;
    sampler2D texture_emission1;     // Alternative name for emissive
    
    // Special effects
    sampler2D texture_height1;
    sampler2D texture_displacement1;
    sampler2D texture_reflection1;
    sampler2D texture_opacity1;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPosition;
uniform bool useParallaxMapping;
uniform float heightScale;

out vec4 FragColor;

// Constants
const float PI = 3.14159265359;

// Check if a texture is provided (non-empty)
bool hasTexture(sampler2D tex) {
    return textureSize(tex, 0).x > 1;
}

// Get color from a texture if available, otherwise use default
vec4 getTextureColor(sampler2D tex, vec2 coords, vec4 defaultColor) {
    if (hasTexture(tex))
        return texture(tex, coords);
    return defaultColor;
}

// Parallax mapping for displacement
vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
    if (!useParallaxMapping || !hasTexture(material.texture_height1))
        return texCoords;
        
    // Number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    
    // Calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    
    // Depth of current layer
    float currentLayerDepth = 0.0;
    
    // The amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
    
    // Get initial values
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = 1.0 - texture(material.texture_height1, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // Shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        
        // Get depth map value at current texture coordinates
        currentDepthMapValue = 1.0 - texture(material.texture_height1, currentTexCoords).r;
        
        // Get depth of next layer
        currentLayerDepth += layerDepth;
    }
    
    // Get texture coordinates before collision (reverse operation)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // Get depth after and before collision for linear interpolation
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (1.0 - texture(material.texture_height1, prevTexCoords).r) - currentLayerDepth + layerDepth;
    
    // Interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
    
    return finalTexCoords;
}

// Get normal vector from normal maps
vec3 getNormalFromMap(vec2 texCoords)
{
    vec3 tangentNormal;
    
    if (hasTexture(material.texture_normal1)) {
        tangentNormal = texture(material.texture_normal1, texCoords).xyz * 2.0 - 1.0;
    }
    else if (hasTexture(material.texture_normal_camera1)) {
        tangentNormal = texture(material.texture_normal_camera1, texCoords).xyz * 2.0 - 1.0;
    }
    else {
        return normalize(Normal);
    }
    
    // Mix with second normal map if available
    if (hasTexture(material.texture_normal2)) {
        vec3 tangentNormal2 = texture(material.texture_normal2, texCoords).xyz * 2.0 - 1.0;
        tangentNormal = mix(tangentNormal, tangentNormal2, 0.5);
    }
    
    return normalize(TBN * tangentNormal);
}

// PBR functions
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return a2 / max(denom, 0.0000001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main()
{
    // View vector (in tangent space for parallax mapping)
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 V = normalize(viewPosition - FragPos);
    
    // Apply parallax mapping if height/displacement map is available
    vec2 texCoords = TexCoords;
    if (hasTexture(material.texture_height1) || hasTexture(material.texture_displacement1)) {
        texCoords = parallaxMapping(TexCoords, viewDir);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }
    
    // Get normal from normal map
    vec3 N = getNormalFromMap(texCoords);
    
    // Base color / albedo - combine diffuse and basecolor maps
    vec4 albedo = vec4(0.8, 0.8, 0.8, 1.0); // Default color
    
    if (hasTexture(material.texture_diffuse1)) {
        albedo = texture(material.texture_diffuse1, texCoords);
        
        // Mix with additional diffuse maps if available
        if (hasTexture(material.texture_diffuse2))
            albedo = mix(albedo, texture(material.texture_diffuse2, texCoords), 0.3);
        if (hasTexture(material.texture_diffuse3))
            albedo = mix(albedo, texture(material.texture_diffuse3, texCoords), 0.2);
        if (hasTexture(material.texture_diffuse4))
            albedo = mix(albedo, texture(material.texture_diffuse4, texCoords), 0.1);
    }
    else if (hasTexture(material.texture_basecolor1)) {
        albedo = texture(material.texture_basecolor1, texCoords);
        
        if (hasTexture(material.texture_basecolor2))
            albedo = mix(albedo, texture(material.texture_basecolor2, texCoords), 0.3);
    }
    
    // Get metalness
    float metalness = 0.0;
    if (hasTexture(material.texture_metalness1)) {
        metalness = texture(material.texture_metalness1, texCoords).r;
        
        if (hasTexture(material.texture_metalness2))
            metalness = mix(metalness, texture(material.texture_metalness2, texCoords).r, 0.5);
    }
    
    // Get roughness (from shininess or derived from metalness)
    float roughness = 0.5;
    if (hasTexture(material.texture_shininess1)) {
        // Convert shininess to roughness (approximation)
        float shininess = texture(material.texture_shininess1, texCoords).r * 100.0;
        roughness = sqrt(2.0 / (shininess + 2.0));
    }
    else {
        // Default: Derive from metalness (can be adjusted)
        roughness = 1.0 - metalness * 0.7;
    }
    
    // Ambient occlusion
    float ao = 1.0;
    if (hasTexture(material.texture_ao1)) {
        ao = texture(material.texture_ao1, texCoords).r;
    }
    else if (hasTexture(material.texture_lightmap1)) {
        // Lightmaps often contain AO information
        ao = texture(material.texture_lightmap1, texCoords).r;
    }
    
    // Ambient lighting
    vec3 ambient = vec3(0.03);
    if (hasTexture(material.texture_ambient1)) {
        ambient = texture(material.texture_ambient1, texCoords).rgb;
        
        if (hasTexture(material.texture_ambient2))
            ambient = mix(ambient, texture(material.texture_ambient2, texCoords).rgb, 0.5);
    }
    
    // Emissive contribution
    vec3 emission = vec3(0.0);
    if (hasTexture(material.texture_emissive1)) {
        emission = texture(material.texture_emissive1, texCoords).rgb;
    }
    else if (hasTexture(material.texture_emission1)) {
        emission = texture(material.texture_emission1, texCoords).rgb;
    }
    
    // Traditional specular value
    vec3 specularColor = vec3(0.5);
    if (hasTexture(material.texture_specular1)) {
        specularColor = texture(material.texture_specular1, texCoords).rgb;
        
        if (hasTexture(material.texture_specular2))
            specularColor = mix(specularColor, texture(material.texture_specular2, texCoords).rgb, 0.5);
    }
    
    // Opacity/transparency
    float opacity = albedo.a;
    if (hasTexture(material.texture_opacity1)) {
        opacity = texture(material.texture_opacity1, texCoords).r;
    }
    
    // Reflection contribution
    vec3 reflection = vec3(0.0);
    if (hasTexture(material.texture_reflection1)) {
        vec3 reflectDir = reflect(-V, N);
        reflection = texture(material.texture_reflection1, vec2(reflectDir.xy)).rgb;
    }
    
    // PBR lighting calculation
    vec3 F0 = vec3(0.04); // Default dielectric fresnel value
    F0 = mix(F0, albedo.rgb, metalness);
    
    // Calculate direct lighting
    vec3 Lo = vec3(0.0);
    
    // Light direction
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L);
    
    // Calculate distance attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    vec3 radiance = light.color * attenuation;
    
    // BRDF calculation
    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);
    
    // For non-PBR pipeline, blend with traditional specular
    if (hasTexture(material.texture_specular1)) {
        float specPower = pow(max(dot(N, H), 0.0), max(8.0 / roughness, 1.0));
        specular = mix(specular, specularColor * specPower, 0.5);
    }
    
    // kS is equal to Fresnel
    vec3 kS = F;
    
    // kD is everything that isn't reflected, scaled by metallicity
    // For metals, there's no diffuse reflection (kD = 0)
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;
    
    // Add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * albedo.rgb / PI + specular) * radiance * NdotL;
    
    // Combine all contributions
    vec3 ambientColor = ambient * albedo.rgb * ao;
    vec3 reflectionContribution = reflection * metalness;
    
    vec3 result = ambientColor + Lo + emission + reflectionContribution;
    
    // HDR tonemapping
    result = result / (result + vec3(1.0));
    
    // Gamma correction
    result = pow(result, vec3(1.0/2.2)); 
    
    FragColor = vec4(result, opacity);
}