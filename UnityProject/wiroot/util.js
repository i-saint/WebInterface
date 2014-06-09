
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
