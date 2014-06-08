
function radians(v)
{
    return v * (Math.PI/180.0);
}

function each(v, f) {
    for (var i = 0; i < v.length; ++i) { f(v[i]); }
}

function clearChildren(e) {
    while(e.lastChild) { e.removeChild(e.lastChild); }
}
function resizeChildren(e, num, creater) {
    while(e.childNodes.length>num) { e.removeChild(e.lastChild); }
    while(e.childNodes.length<num) { e.appendChild(creater()); }
}


function bool_s(v)  { return "bool("+v.toString()+")"; }
function int32_s(v)   { return "int32("+v.toString()+")"; }
function uint32_s(v)  { return "uint32("+v.toString()+")"; }
function float32_s(v) { return "float32("+v.toString()+")"; }
function vec2_s(v)  { return "vec2("+v[0].toString()+","+v[1].toString()+")"; }
function vec3_s(v)  { return "vec3("+v[0].toString()+","+v[1].toString()+","+v[2].toString()+")"; }
function vec4_s(v)  { return "vec4("+v[0].toString()+","+v[1].toString()+","+v[2].toString()+","+v[3].toString()+")"; }
function string_s(v){ return "string(\""+v+"\")"; }
function instruction_s(p,e) { return "instruction("+p[0].toString()+","+p[1].toString()+",0.0,"+e.toString()+")"; }
function curvepoint_s(p) { return "curvepoint("+p[0].toFixed(2)+","+p[1].toFixed(2)+","+p[2].toFixed(2)+","+p[3].toFixed(2)+","+p[4].toString()+")"; }

function createShader(id)
{
    var shaderScript = document.getElementById(id);
    if(!shaderScript) { return null; }

    var str = "";
    var k = shaderScript.firstChild;
    while (k) {
        if (k.nodeType == 3) {
            str += k.textContent;
        }
        k = k.nextSibling;
    }

    var shader;
    if(shaderScript.type == "x-shader/x-fragment") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    }
    else if(shaderScript.type == "x-shader/x-vertex") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    }
    else {
        return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);
    if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        return null;
    }
    return shader;
}

function createShaderProgram(vsid, psid)
{
    var program = gl.createProgram();
    gl.attachShader(program, createShader(vsid));
    gl.attachShader(program, createShader(psid));
    gl.linkProgram(program);
    return program;
}

