uniform sampler2D texture;
uniform vec3 camera_position;
uniform int grid_size;
uniform vec2 mouse_position;

const vec4 camera_direction = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec2 fragPosition = gl_FragCoord.xy;

    vec3 rayDirection = normalize(
        vec4(vec4(fragPosition, 0.0, 1.0) * camera_direction).xyz
        - vec4(vec4(800 / 2, 600 / 2, -400, 1.0) * camera_direction).xyz
    );

    vec3 mouseDirection = normalize(
        vec4(vec4(mouse_position, 0.0, 1.0) * camera_direction).xyz
        - vec4(vec4(800 / 2, 600 / 2, -400, 1.0) * camera_direction).xyz
    );

    mouseDirection.y = -mouseDirection.y;
    ivec3 space_mouse = ivec3(floor(camera_position + ((-camera_position.z / mouseDirection.z)-0.5) * mouseDirection));

    vec3 rayPos = camera_position + ((-camera_position.z / rayDirection.z)-0.5) * rayDirection;
    ivec3 mapPos = ivec3(floor(rayPos));
    vec3 deltaDist = abs(vec3(length(rayDirection)) / rayDirection);
    ivec3 rayStep = ivec3(sign(rayDirection));
    vec3 sideDist = (sign(rayDirection) * (vec3(mapPos) - rayPos) + (sign(rayDirection) * 0.5) + 0.5) * deltaDist;

    int face = 0;
    int steps = 5;

    for(int i = 0; i<steps; i++){
        if (mapPos.x >= 0 && mapPos.x <= (grid_size - 1) &&
            mapPos.y >= 0 && mapPos.y <= (grid_size - 1) &&
            mapPos.z >= 0 && mapPos.z <= (grid_size - 1)) {
            vec4 pixel = texture2D(texture, vec2(mapPos.xy+0.5)/float(grid_size));
            if(pixel.w > 0.0 && mapPos.z == 0){
                
                vec3 light = vec3(0.2*face);

                gl_FragColor = vec4(mix(rayDirection, light, 1.2), 1.0);
                return;
            }
            if(mapPos.z == 0 && length(vec3(space_mouse-mapPos))<=1.0){
                gl_FragColor = vec4(1.0, 1.0, 1.0, 0.75);
                return;
            }
        }

        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                sideDist.x += deltaDist.x;
                mapPos.x += rayStep.x;
                face = (rayStep.x > 0) ? 0 : 3;
            } else {
                sideDist.z += deltaDist.z;
                mapPos.z += rayStep.z;
                face = (rayStep.z > 0) ? 2 : 5;
            }
        } else {
            if (sideDist.y < sideDist.z) {
                sideDist.y += deltaDist.y;
                mapPos.y += rayStep.y;
                face = (rayStep.y > 0) ? 1 : 4;
            } else {
                sideDist.z += deltaDist.z;
                mapPos.z += rayStep.z;
                face = (rayStep.z > 0) ? 2 : 5;
            }
        }
    }

    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.5);

}