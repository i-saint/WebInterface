var sh_entities;
var sh_particles;
var sh_points;
var sh_lasers;
var vb_quad;
var ib_quad;
var ib_quad_line;
var vb_cube;
var ib_cube;
var ib_cube_line;
var vb_bullets;
var vb_lasers;
var vb_fluids;
var vb_curve;
var vb_curve_points;

function initializeGLResources()
{
    sh_entities = createShaderProgram("vs_entities", "ps_entities");
    sh_entities.ia_position = gl.getAttribLocation(sh_entities, "ia_position");
    sh_entities.u_proj = gl.getUniformLocation(sh_entities, "u_proj");
    sh_entities.u_trans = gl.getUniformLocation(sh_entities, "u_trans");
    sh_entities.u_size = gl.getUniformLocation(sh_entities, "u_size");
    sh_entities.u_color = gl.getUniformLocation(sh_entities, "u_color");

    sh_particles = createShaderProgram("vs_particles", "ps_particles");
    sh_particles.ia_position = gl.getAttribLocation(sh_particles, "ia_position");
    sh_particles.u_proj = gl.getUniformLocation(sh_particles, "u_proj");
    sh_particles.u_pointSize = gl.getUniformLocation(sh_particles, "u_pointSize");
    sh_particles.u_color = gl.getUniformLocation(sh_particles, "u_color");

    sh_points = createShaderProgram("vs_points", "ps_points");
    sh_points.ia_position = gl.getAttribLocation(sh_points, "ia_position");
    sh_points.ia_color = gl.getAttribLocation(sh_points, "ia_color");
    sh_points.u_proj = gl.getUniformLocation(sh_points, "u_proj");
    sh_points.u_pointSize = gl.getUniformLocation(sh_points, "u_pointSize");


    var quad_vertices = new Float32Array([
        -1.0, -1.0,
        -1.0, 1.0,
         1.0, 1.0,
         1.0, -1.0
    ]);
    vb_quad = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vb_quad);
    gl.bufferData(gl.ARRAY_BUFFER, quad_vertices, gl.STATIC_DRAW);

    var quad_indices = new Int16Array([0, 1, 2, 2, 3, 0]);
    ib_quad = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_quad);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, quad_indices, gl.STATIC_DRAW);

    var quad_line_indices = new Int16Array([0, 1, 1, 2, 2, 3, 3, 0]);
    ib_quad_line = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_quad_line);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, quad_line_indices, gl.STATIC_DRAW);


    var cube_vertices = new Float32Array([
         1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
         1.0, -1.0, 1.0,
         1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
    ]);
    vb_cube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vb_cube);
    gl.bufferData(gl.ARRAY_BUFFER, cube_vertices, gl.STATIC_DRAW);

    var cube_indices = new Int16Array([
        0, 1, 2, 2, 3, 0,
        0, 4, 5, 5, 1, 0,
        2, 1, 5, 5, 6, 2,
        3, 7, 4, 4, 0, 3,
        3, 2, 6, 6, 7, 3,
        7, 6, 5, 5, 4, 7,
    ]);
    ib_cube = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_cube);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, cube_indices, gl.STATIC_DRAW);

    var cube_line_indices = new Int16Array([
        0, 1, 1, 2, 2, 3, 3, 0,
        0, 4, 4, 5, 5, 1, 1, 0,
        2, 1, 1, 5, 5, 6, 6, 2,
        3, 7, 7, 4, 4, 0, 0, 3,
        3, 2, 2, 6, 6, 7, 7, 3,
        7, 6, 6, 5, 5, 4, 4, 7,
    ]);
    ib_cube_line = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_cube_line);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, cube_line_indices, gl.STATIC_DRAW);
}


function drawGL() {
    camera.updateMatrix();
    var projection = camera.viewProj;

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.enable(gl.CULL_FACE);
    gl.cullFace(gl.BACK);
    gl.enable(gl.BLEND);

    gl.blendFunc(gl.SRC_ALPHA, gl.ONE);
    gl.useProgram(sh_entities);
    gl.bindBuffer(gl.ARRAY_BUFFER, vb_cube);
    gl.enableVertexAttribArray(sh_entities.ia_position);
    gl.vertexAttribPointer(sh_entities.ia_position, 3, gl.FLOAT, gl.FALSE, 12, 0);
    gl.uniformMatrix4fv(sh_entities.u_proj, gl.FALSE, projection);
    for (var i = 0; i < entities.ids.length; ++i) {
        var ipp = i + 1;
        gl.uniformMatrix4fv(sh_entities.u_trans, gl.FALSE, entities.trans.subarray(16 * i, 16 * ipp));
        gl.uniform3fv(sh_entities.u_size, entities.size.subarray(3 * i, 3 * ipp));
        if (editor.entitySelection.include(entities.ids[i])) {
            gl.uniform4fv(sh_entities.u_color, [1, 1, 1, 0.3]);
        }
        else {
            gl.uniform4fv(sh_entities.u_color, entities.color.subarray(4 * i, 4 * ipp));
        }
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_cube);
        gl.drawElements(gl.TRIANGLES, 36, gl.UNSIGNED_SHORT, 0);
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ib_cube_line);
        gl.drawElements(gl.LINES, 48, gl.UNSIGNED_SHORT, 0);
    }



    gl.flush();
}
