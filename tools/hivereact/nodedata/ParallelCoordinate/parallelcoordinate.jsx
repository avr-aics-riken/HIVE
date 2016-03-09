import React from 'react';
import ReactDOM from "react-dom";

function zeroPadding(n, c){
    return (new Array(c + 1).join('0') + n).slice(-c);
}

class prgLocations {
    constructor(props){
        this.prg = null;
        this.vs  = null;
        this.fs  = null;
        this.vSource = '';
        this.fSource = '';
        this.attL = [];
        this.attS = [];
        this.uniL = {};
        this.horizonBuffer = null;
        this.verticalBuffer = null;
        this.bufferWidth = 0;
        this.bufferHeight = 0;
    }
}

// ============================================================================
// this.dimensionTitles === 上部に出る項目タイトルを格納した配列（未指定の場合配列インデックス）
// ============================================================================
class ParallelCoordinate extends React.Component {
    constructor(props) {
        super(props);

        // member
        this.store = this.props.store;
        this.action = this.props.action;
        this.node = this.props.node;

        this.mat = new matIV(); // from App/lib/glutil.js
        this.glContext = {};    // cache
        this.prev = {
            prevType: null,
            glforeground: null,
            glbrush: null
        };
        this.usr = {glRender: this.glRender};
        this.parcoords;         // from d3.parcoord.js
        this.dataval = null;
        this.density = false;
        this.densityNormalize = false;
        this.weight = [];
        this.linecount = 0;
        this.dimensionTitles = {};

        // state
        this.state = {
            parse: null,
            data: null,
            param: null
        };

        // method
        this.redraw = this.redraw.bind(this);
        this.useAxes = this.useAxes.bind(this);
        this.beginDraw = this.beginDraw.bind(this);
        this.glInitialize = this.glInitialize.bind(this);
        this.canvasAttCopy = this.canvasAttCopy.bind(this);
        this.glRender = this.glRender.bind(this);
        this.singleConv = this.singleConv.bind(this);
        this.imageRecieved = this.imageRecieved.bind(this);
        this.imageParse = this.imageParse.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentDidUpdate = this.componentDidUpdate.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);

        // event
        this.onChangeDensity = this.onChangeDensity.bind(this);
        this.onChangeDensityNormalize = this.onChangeDensityNormalize.bind(this);
        this.onColorChange = this.onColorChange.bind(this);
    }

    imageRecieved(err, param, data){
        var buffer;
        const varname = this.node.varname;
        if (param.varname !== varname) {
            return;
        }
        let a
        if (param.datatype === "byte") {
            a = new Uint8Array(data);
            console.log('byte');
        } else if (param.datatype === 'float'){
            a = new Float32Array(data);
            console.log('float');
        }
        let component = parseInt(param.component, 10);
        let parse = [];
        for(let i = 0, j = a.length / component; i < j; ++i){
            let k = i * component;
            parse.push([a[k], a[k + 1], a[k + 2]]);
        }
        this.setState({
            parse: parse,
            data: data,
            param: param
        });
        setTimeout(this.imageParse, 50);
    }

    imageParse(){
        if(this.state.parse === null || this.state.parse === undefined){return;}
        if(this.state.parse[0].length > 2){
            this.useAxes();
        }else{
            console.log('parallel: invalid data');
        }
    }

    onChangeDensity(){
        this.density = !this.density;
        this.setState({density: this.density});
        setTimeout((()=>{this.redraw();}).bind(this), 50);
    }

    onChangeDensityNormalize(){
        this.densityNormalize = !this.densityNormalize;
        this.setState({densityNormalize: this.densityNormalize});
        setTimeout((()=>{this.redraw();}).bind(this), 50);
    }

    onColorChange(eve){
        var c, e, r, g, b;
        var a = [];
        for(let i = 1; i <= 2; ++i){
            e = ReactDOM.findDOMNode(this.refs["lineColor" + i]);
            c = e.value.match(/[0-9|a-f]{2}/ig);
            r = parseInt(c[0], 16) / 255;
            g = parseInt(c[1], 16) / 255;
            b = parseInt(c[2], 16) / 255;
            a.push([r, g, b, this.props.node.input[i - 1].value[3]]);
        }
        // send action from color input change
        this.props.action.changeNodeInput({
            varname: this.props.node.varname,
            input: {
                lineColor1: a[0],
                lineColor2: a[1]
            }
        });
        this.redraw();
    }

    singleConv(color){
        let r = zeroPadding(new Number(parseInt(color[0] * 255)).toString(16), 2);
        let g = zeroPadding(new Number(parseInt(color[1] * 255)).toString(16), 2);
        let b = zeroPadding(new Number(parseInt(color[2] * 255)).toString(16), 2);
        return '#' + r + g + b;
    }

    redraw(){
        var f = false;
        if(this.prev.prevType != null){
            if(this.prev.glbrush != null && this.prev.glbrush.data != null){
                f = true;
                this.glRender(
                    'glbrush',
                    this.prev.glbrush.data,
                    this.prev.glbrush.lines,
                    this.prev.glbrush.left,
                    this.prev.glbrush.right
                );
            }
            if(this.prev.glforeground != null && this.prev.glforeground.data != null){
                f = true;
                this.glRender(
                    'glforeground',
                    this.prev.glforeground.data,
                    this.prev.glforeground.lines,
                    this.prev.glforeground.left,
                    this.prev.glforeground.right
                );
            }
        }
        if(!f){this.useAxes();}
    }

    useAxes(){
        let e = ReactDOM.findDOMNode(this.refs.examples);
        if(e){e.innerHTML = '';}
        if(this.state.parse === null){
            // dom reset
            // d3.csv('./App/resource/nut.csv', (function(data){
            //     this.state.parse = data;
            //     this.beginDraw(this.state.parse);
            // }).bind(this));
        }else{
            this.beginDraw(this.state.parse);
        }
    }

    beginDraw(data){
        if(data.length < 3){console.log('invalid data:' + data); return;}
        this.dimensionTitles = {};
        this.dataval = [];
        if(Array.isArray(data[0])){ // csv 先頭行がタイトルではない
            for(let i = 0, j = data[0].length; i < j; ++i){
                this.dimensionTitles[i] = i;
            }
            for(let i = 0, j = data.length; i < j; ++i){
                this.dataval[i] = [];
                for(let k = 0, l = data[i].length; k < l; ++k){
                    this.dataval[i][k] = data[i][k];
                }
            }
        }else{ // csv 先頭行がタイトルになっており配列の中に javascript オブジェクトが入っているケース
            let i = 0;
            for(let j in data[0]){
                this.dimensionTitles[i] = j;
                i++;
            }
            for(let i = 0, j = data.length; i < j; ++i){
                this.dataval[i] = [];
                for(let k in this.dimensionTitles){
                    this.dataval[i][k] = data[i][this.dimensionTitles[k]];
                }
            }
        }
        this.usr = {glRender: this.glRender};
        this.linecount = this.dataval.length;
        this.parcoords = d3.parcoords({dimensionTitles: this.dimensionTitles, usr: this.usr})(ReactDOM.findDOMNode(this.refs.examples))
            .data(this.dataval)   // データの代入
            .mode("queue")        // 描画を WebGL Renderer に
            .width(500)           // 描画エリアのサイズ（横幅）
            .height(320);         // 描画エリアのサイズ（縦）

        this.glInitialize();
        this.parcoords.render()   // ラインを描画する
            .createAxes()         // 目盛を生成する
            .reorderable()        // 軸の並び替え有効化
            .brushMode("1D-axes") // 抽出のやり方
            .interactive();       // 常時更新
    }
    glInitialize(){
        if(this.parcoords == null){return;}
        if(!document.getElementById('glforeground')){
            this.glContext = {};
            var e = this.parcoords.selection.node();
            var m = this.parcoords.canvas.marks;
            var c = document.createElement('canvas');
            this.canvasAttCopy(c, 'glbrush', m);
            this.glContext['glbrush'].color = this.props.node.input[1].value;
            // this.glContext['glbrush'].lowColor        = this.colors.brush[0];
            // this.glContext['glbrush'].middleLowColor  = this.colors.brush[1];
            // this.glContext['glbrush'].middleColor     = this.colors.brush[2];
            // this.glContext['glbrush'].middleHighColor = this.colors.brush[3];
            // this.glContext['glbrush'].highColor       = this.colors.brush[4];
            e.insertBefore(c, e.firstChild);
            c = document.createElement('canvas');
            this.canvasAttCopy(c, 'glforeground', m);
            this.glContext['glforeground'].color = this.props.node.input[0].value;
            // this.glContext['glforeground'].lowColor        = this.colors.foreground[0];
            // this.glContext['glforeground'].middleLowColor  = this.colors.foreground[1];
            // this.glContext['glforeground'].middleColor     = this.colors.foreground[2];
            // this.glContext['glforeground'].middleHighColor = this.colors.foreground[3];
            // this.glContext['glforeground'].highColor       = this.colors.foreground[4];
            e.insertBefore(c, e.firstChild);
        }
    }
    canvasAttCopy(c, name, m){
        c.id = name;
        c.style.cssText = m.style.cssText;
        c.width = m.width;
        c.height = m.height;

        if(this.glContext[name] == null){
            this.glContext[name] = {
                gl: c.getContext('webgl'),
                color:           [0.2, 0.2, 0.2, 0.1],
                lowColor:        [1.0, 1.0, 1.0],
                middleLowColor:  [0.2, 0.2, 0.2],
                middleColor:     [0.1, 0.5, 0.3],
                middleHighColor: [0.6, 0.6, 0.2],
                highColor:       [0.8, 0.2, 0.1],
                pl:  new prgLocations(),
                plp: new prgLocations(),
                plf: new prgLocations()
            };
        }
    }

    glRender(target, data, lines, left, right){
        this.prev.prevType = target;
        this.prev[target] = {target: target, data: data, lines: lines, left: left, right: right};
        if(this.glContext[target].gl == null){return;}

        var gc = this.glContext[target];
        var gl = gc.gl;
        var vPosition, vboL;
        var polyPosition = [];
        var vPolyPosition, vboPL;
        var width = gl.canvas.width;
        var height = gl.canvas.height;
        var ext;
        var mat = this.mat;
        ext = gl.getExtension('OES_texture_float');

        if(gc.pl.prg == null){
            gc.pl.vSource = '';
            gc.pl.vSource += 'attribute vec2 position;';
            gc.pl.vSource += 'uniform mat4 matrix;';
            gc.pl.vSource += 'void main(){';
            gc.pl.vSource += '    gl_Position = matrix * vec4(position, 0.0, 1.0);';
            gc.pl.vSource += '}';

            gc.pl.fSource = '';
            gc.pl.fSource += 'precision mediump float;';
            gc.pl.fSource += 'uniform vec4 color;';
            gc.pl.fSource += 'uniform float density;';
            gc.pl.fSource += 'void main(){';
            gc.pl.fSource += '    if(density > 0.0){';
            gc.pl.fSource += '        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);';
            gc.pl.fSource += '    }else{';
            gc.pl.fSource += '        gl_FragColor = color;';
            gc.pl.fSource += '    }';
            gc.pl.fSource += '}';

            gc.pl.vs = create_shader(gl, gc.pl.vSource, gl.VERTEX_SHADER);
            gc.pl.fs = create_shader(gl, gc.pl.fSource, gl.FRAGMENT_SHADER);
            gc.pl.prg = create_program(gl, gc.pl.vs, gc.pl.fs);

            gc.pl.attL = [gl.getAttribLocation(gc.pl.prg, 'position')];
            gc.pl.attS = [2];
            gc.pl.uniL = {
                matrix: gl.getUniformLocation(gc.pl.prg, 'matrix'),
                color: gl.getUniformLocation(gc.pl.prg, 'color'),
                density: gl.getUniformLocation(gc.pl.prg, 'density')
            };

            gc.plp.vSource = '';
            gc.plp.vSource += 'attribute vec3 position;';
            gc.plp.vSource += 'void main(){';
            gc.plp.vSource += '    gl_Position = vec4(position, 1.0);';
            gc.plp.vSource += '}';

            gc.plp.fSource = '';
            gc.plp.fSource += 'precision mediump float;';
            gc.plp.fSource += 'uniform vec2 resolution;';
            gc.plp.fSource += 'uniform bool horizontal;';
            gc.plp.fSource += 'uniform float weight[30];';
            gc.plp.fSource += 'uniform sampler2D texture;';
            gc.plp.fSource += 'void main(){';
            gc.plp.fSource += '    vec2 tFrag = 1.0 / resolution;';
            gc.plp.fSource += '    vec2 fc = gl_FragCoord.st;';
            gc.plp.fSource += '    vec4 destColor = texture2D(texture, fc) * weight[0];';
            gc.plp.fSource += '    if(horizontal){';
            gc.plp.fSource += '        for(int i = 1; i < 30; ++i){';
            gc.plp.fSource += '            destColor += texture2D(texture, (fc + vec2( float(i), 0.0)) * tFrag) * weight[i];';
            gc.plp.fSource += '            destColor += texture2D(texture, (fc + vec2(-float(i), 0.0)) * tFrag) * weight[i];';
            gc.plp.fSource += '        }';
            gc.plp.fSource += '    }else{';
            gc.plp.fSource += '        for(int i = 1; i < 30; ++i){';
            gc.plp.fSource += '            destColor += texture2D(texture, (fc + vec2(0.0,  float(i))) * tFrag) * weight[i];';
            gc.plp.fSource += '            destColor += texture2D(texture, (fc + vec2(0.0, -float(i))) * tFrag) * weight[i];';
            gc.plp.fSource += '        }';
            gc.plp.fSource += '    }';
            gc.plp.fSource += '    gl_FragColor = destColor;';
            gc.plp.fSource += '}';

            gc.plp.vs = create_shader(gl, gc.plp.vSource, gl.VERTEX_SHADER);
            gc.plp.fs = create_shader(gl, gc.plp.fSource, gl.FRAGMENT_SHADER);
            gc.plp.prg = create_program(gl, gc.plp.vs, gc.plp.fs);

            gc.plp.attL = [gl.getAttribLocation(gc.plp.prg, 'position')];
            gc.plp.attS = [3];
            gc.plp.uniL = {
                resolution: gl.getUniformLocation(gc.plp.prg, 'resolution'),
                horizontal: gl.getUniformLocation(gc.plp.prg, 'horizontal'),
                weight:     gl.getUniformLocation(gc.plp.prg, 'weight'),
                texture:    gl.getUniformLocation(gc.plp.prg, 'texture')
            };

            gc.plf.vSource = '';
            gc.plf.vSource += 'attribute vec3 position;';
            gc.plf.vSource += 'void main(){';
            gc.plf.vSource += '    gl_Position = vec4(position, 1.0);';
            gc.plf.vSource += '}';

            gc.plf.fSource = '';
            gc.plf.fSource += 'precision mediump float;';
            gc.plf.fSource += 'uniform vec4 color;';
            gc.plf.fSource += 'uniform vec2 resolution;';
            gc.plf.fSource += 'uniform sampler2D texture;';
            gc.plf.fSource += 'uniform float density;';
            gc.plf.fSource += 'uniform vec3 lowColor;';
            gc.plf.fSource += 'uniform vec3 middleLowColor;';
            gc.plf.fSource += 'uniform vec3 middleColor;';
            gc.plf.fSource += 'uniform vec3 middleHighColor;';
            gc.plf.fSource += 'uniform vec3 highColor;';
            gc.plf.fSource += 'const float low = 0.2;';
            gc.plf.fSource += 'const float middle = 0.4;';
            gc.plf.fSource += 'const float high = 0.7;';
            gc.plf.fSource += 'void main(){';
            gc.plf.fSource += '    if(density > 0.0){';
            gc.plf.fSource += '        vec4 c = color;';
            gc.plf.fSource += '        vec2 texcoord = gl_FragCoord.st / resolution;';
            gc.plf.fSource += '        vec4 smpColor = texture2D(texture, texcoord);';
            gc.plf.fSource += '        float range = smpColor.a / density;';
            gc.plf.fSource += '        if(range < low){';
            gc.plf.fSource += '            c = vec4(mix(lowColor, middleLowColor, smoothstep(0.0, low, range)) * 1.5, 1.0);';
            gc.plf.fSource += '        }else if(range < middle){';
            gc.plf.fSource += '            c = vec4(mix(middleLowColor, middleColor, smoothstep(low, middle, range)) * 1.5, 1.0);';
            gc.plf.fSource += '        }else if(range < high){';
            gc.plf.fSource += '            c = vec4(mix(middleColor, middleHighColor, smoothstep(middle, high, range)) * 1.5, 1.0);';
            gc.plf.fSource += '        }else{';
            gc.plf.fSource += '            c = vec4(mix(middleHighColor, highColor, smoothstep(high, 1.0, range)) * 1.5, 1.0);';
            gc.plf.fSource += '        }';
            gc.plf.fSource += '        gl_FragColor = vec4(c.rgb, range * 2.0);';
            gc.plf.fSource += '    }else{';
            gc.plf.fSource += '        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);';
            gc.plf.fSource += '    }';
            gc.plf.fSource += '}';

            gc.plf.vs = create_shader(gl, gc.plf.vSource, gl.VERTEX_SHADER);
            gc.plf.fs = create_shader(gl, gc.plf.fSource, gl.FRAGMENT_SHADER);
            gc.plf.prg = create_program(gl, gc.plf.vs, gc.plf.fs);

            gc.plf.attL = [gl.getAttribLocation(gc.plf.prg, 'position')];
            gc.plf.attS = [3];
            gc.plf.uniL = {
                color: gl.getUniformLocation(gc.plf.prg, 'color'),
                resolution: gl.getUniformLocation(gc.plf.prg, 'resolution'),
                texture: gl.getUniformLocation(gc.plf.prg, 'texture'),
                density: gl.getUniformLocation(gc.plf.prg, 'density'),
                lowColor: gl.getUniformLocation(gc.plf.prg, 'lowColor'),
                middleLowColor: gl.getUniformLocation(gc.plf.prg, 'middleLowColor'),
                middleColor: gl.getUniformLocation(gc.plf.prg, 'middleColor'),
                middleHighColor: gl.getUniformLocation(gc.plf.prg, 'middleHighColor'),
                highColor: gl.getUniformLocation(gc.plf.prg, 'highColor')
            };

            (()=>{
                var t = 0.0;
                for(var i = 0; i < 30; i++){
                    var r = 1.0 + 2.0 * i;
                    var w = Math.exp(-0.5 * (r * r) / 150.0);
                    this.weight[i] = w;
                    if(i > 0){w *= 2.0;}
                    t += w;
                }
                for(i = 0; i < this.weight.length; i++){
                    this.weight[i] /= t;
                }
            })();

            (()=>{
                var i;
                for(i = 1; i < width; i *= 2){}
                gc.plp.bufferWidth = Math.max(i, 1024);
                for(i = 1; i < height; i *= 2){}
                gc.plp.bufferHeight = Math.max(Math.max(i, 1024), gc.plp.bufferWidth);
            })();
            gc.plp.horizonBuffer  = create_framebuffer(gl, ext, gc.plp.bufferWidth, gc.plp.bufferHeight);
            gc.plp.verticalBuffer = create_framebuffer(gl, ext, gc.plp.bufferWidth, gc.plp.bufferHeight);
        }

        gl.enable(gl.BLEND);
        gl.blendFuncSeparate(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA, gl.ONE, gl.ONE);
        // gl.blendFuncSeparate(gl.ONE, gl.ONE, gl.ONE, gl.ONE);
        gl.lineWidth(1.5);
        gl.useProgram(gc.pl.prg);
        gl.viewport(0, 0, width, height);
        gl.clearColor(0.0, 0.0, 0.0, 0.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        if(data == null){return;}
        // data をアウトプットに出すとしたらここぽい気がする

        var vMatrix = mat.identity(mat.create());
        var pMatrix = mat.identity(mat.create());
        var vpMatrix = mat.identity(mat.create());

        vPosition = create_vbo(gl, data);
        vboL = [vPosition];

        polyPosition = [
            -1.0,  1.0,  0.0,
             1.0,  1.0,  0.0,
            -1.0, -1.0,  0.0,
             1.0, -1.0,  0.0
        ];
        vPolyPosition = create_vbo(gl, polyPosition);
        vboPL = [vPolyPosition];

        mat.lookAt(
            [0.0, 0.0, 1.0],
            [0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0],
            vMatrix
        );
        mat.ortho(
            0,
            width,
            0,
            height,
            0.5,
            5.0,
            pMatrix
        );
        mat.multiply(pMatrix, vMatrix, vpMatrix);

        if(this.densityNormalize){
            lines *= (101 - this.densityRange) / 100 * 0.5;
        }else{
            lines = this.linecount * (101 - this.densityRange) / 100 * 0.5;
        }

        if(this.density){
            // first scene to vertical buffer
            gl.bindFramebuffer(gl.FRAMEBUFFER, gc.plp.verticalBuffer.framebuffer);
            gl.viewport(0, 0, gc.plp.bufferWidth, gc.plp.bufferHeight);
            gl.clearColor(0.0, 0.0, 0.0, 0.0);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.useProgram(gc.pl.prg);
            set_attribute(gl, vboL, gc.pl.attL, gc.pl.attS);
            gl.uniformMatrix4fv(gc.pl.uniL.matrix, false, vpMatrix);
            gl.uniform4fv(gc.pl.uniL.color, gc.color);
            // gl.uniform1f(gc.pl.uniL.density, Math.min(lines, 450));
            gl.uniform1f(gc.pl.uniL.density, lines);
            gl.drawArrays(gl.LINES, 0, data.length / 2);

            // horizon blur
            gl.bindFramebuffer(gl.FRAMEBUFFER, gc.plp.horizonBuffer.framebuffer);
            gl.bindTexture(gl.TEXTURE_2D, gc.plp.verticalBuffer.texture);
            gl.viewport(0, 0, gc.plp.bufferWidth, gc.plp.bufferHeight);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.useProgram(gc.plp.prg);
            set_attribute(gl, vboPL, gc.plp.attL, gc.plp.attS);
            gl.uniform2fv(gc.plp.uniL.resolution, [gc.plp.bufferWidth, gc.plp.bufferHeight]);
            gl.uniform1i(gc.plp.uniL.horizontal, true);
            gl.uniform1fv(gc.plp.uniL.weight, this.weight);
            gl.uniform1i(gc.plp.uniL.texture, 0);
            gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

            // vertical blur
            gl.bindFramebuffer(gl.FRAMEBUFFER, gc.plp.verticalBuffer.framebuffer);
            gl.bindTexture(gl.TEXTURE_2D, gc.plp.horizonBuffer.texture);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.uniform1i(gc.plp.uniL.horizontal, false);
            gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

            // first scene to vertical buffer
            gl.bindFramebuffer(gl.FRAMEBUFFER, null);
            gl.bindTexture(gl.TEXTURE_2D, gc.plp.verticalBuffer.texture);
            gl.viewport(0, 0, width, height);
            gl.clearColor(0.0, 0.0, 0.0, 0.0);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.useProgram(gc.plf.prg);
            set_attribute(gl, vboPL, gc.plf.attL, gc.plf.attS);
            gl.uniform4fv(gc.plf.uniL.color, gc.color);
            gl.uniform2fv(gc.plf.uniL.resolution, [width, height]);
            gl.uniform1i(gc.plf.uniL.texture, 0);
            gl.uniform1f(gc.plf.uniL.density, lines);
            gl.uniform3fv(gc.plf.uniL.lowColor, gc.lowColor);
            gl.uniform3fv(gc.plf.uniL.middleLowColor, gc.middleLowColor);
            gl.uniform3fv(gc.plf.uniL.middleColor, gc.middleColor);
            gl.uniform3fv(gc.plf.uniL.middleHighColor, gc.middleHighColor);
            gl.uniform3fv(gc.plf.uniL.highColor, gc.highColor);
            gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

        }else{
            gl.bindFramebuffer(gl.FRAMEBUFFER, null);
            gl.viewport(0, 0, width, height);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.useProgram(gc.pl.prg);
            set_attribute(gl, vboL, gc.pl.attL, gc.pl.attS);
            gl.uniformMatrix4fv(gc.pl.uniL.matrix, false, vpMatrix);
            gl.uniform4fv(gc.pl.uniL.color, gc.color);
            gl.uniform1f(gc.pl.uniL.density, -1.0);
            gl.drawArrays(gl.LINES, 0, data.length / 2);
        }
        gl.flush();
    }

    componentDidMount(){
        const Store_IMAGE_RECIEVED = "image_revieved";
        this.store.on(Store_IMAGE_RECIEVED, this.imageRecieved);

        // state.parse は最初の次元が縦方向にマッピングされる
        // 横棒の折れ線グラフが描かれるイメージ
        // 以下のようにすれば、100 本の線が、それぞれ 10 項目をマッピングする形で描画される
        // this.state.parse = [];
        // for(let i = 0; i < 100; ++i){
        //     this.state.parse[i] = [];
        //     for(let j = 0; j < 10; ++j){
        //         this.state.parse[i].push(Math.random() * 100.0);
        //     }
        // }
        // this.setState(
        //     {parse: [
        //         [0.1, 0.3, 0.5, 0.7, 0.9],
        //         [0.1, 0.3, 0.5, 0.7, 0.9],
        //         [0.1, 0.3, 0.5, 0.7, 0.9],
        //         [0.1, 0.3, 0.5, 0.7, 0.9],
        //         [0.1, 0.3, 0.5, 0.7, 0.9]
        //     ]}
        // );
        // setTimeout((()=>{this.imageParse();}), 50);
        this.glInitialize();
    }

    componentDidUpdate(){
        if(this.glContext.hasOwnProperty('glforeground')){
            this.glContext['glforeground'].color = this.props.node.input[0].value;
            this.glContext['glbrush'].color = this.props.node.input[1].value;
        }
    }

    componentWillUnmount(){
        const Store_IMAGE_RECIEVED = "image_revieved";
        this.store.removeListener(Store_IMAGE_RECIEVED, this.imageRecieved);
        //this.glContext['glforeground'].color = this.props.node.input[0].value;
        //this.glContext['glbrush'].color = this.props.node.input[1].value;
    }

    styles(){
        return {
            container: {
                width: "500px",
                height: "400px",
                margin: "2px 5px"
            },
            examples: {
                backgroundColor: "white",
                width: "500px",
                height: "320px"
            },
            uiFrame: {
                width: "100%",
                height: "80px",
                display: "flex",
                flexDirection: "column"
            },
            flexrow: {
                flex: "1 0 auto",
                padding: "2px",
                display: "flex",
                flexDirection: "row"
            },
            flexcol: {
                flex: "1 0 auto",
                padding: "2px",
                display: "flex",
                flexDirection: "row"
            },
            colorInputs: {
                width: "20px",
                height: "20px",
                padding: "0px"
            },
            inputTitle: {
                marginRight: "3px",
            },
            canvas: {},
        };
    }

    render(){
        const styles = this.styles();
        return (
            <div>
                <div ref="container" style={styles.container}>
                    <div ref="examples" className="parcoords" style={styles.examples}></div>
                    <div style={styles.uiFrame}>
                        <div style={styles.flexrow}>
                            <div style={styles.flexcol}>
                                <p style={styles.inputTitle}>line</p>
                                <input type="color" id="color1" ref="lineColor1" value={this.singleConv(this.props.node.input[0].value)} onChange={this.onColorChange} style={styles.colorInputs} />
                                <input type="color" id="color2" ref="lineColor2" value={this.singleConv(this.props.node.input[1].value)} onChange={this.onColorChange} style={styles.colorInputs} />
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}

module.exports = ParallelCoordinate;

            // <div>
            //     <div ref="container" style={styles.container}>
            //         <div ref="examples" className="parcoords" style={styles.examples}></div>
            //         <div style={styles.uiFrame}>
            //             <div style={styles.flexrow}>
            //                 <div style={styles.flexcol}>
            //                     <input type="checkbox" checked={this.state.density} id="densityCheck" onChange={this.onChangeDensity} />
            //                     <label onClick={this.onChangeDensity}>density mode</label>
            //                 </div>
            //                 <div style={styles.flexcol}>
            //                     <input type="checkbox" checked={this.state.densityNormalize} id="densityNormalize" onChange={this.onChangeDensityNormalize} />
            //                     <label onClick={this.onChangeDensityNormalize}>density normalize</label>
            //                 </div>
            //             </div>
            //             <div style={styles.flexrow}>
            //                 <div style={styles.flexcol}>
            //                     <p style={styles.inputTitle}>line</p>
            //                     <input type="color" id="color0" ref="lineColor1" value={this.state.colorString0} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color1" ref="lineColor2" value={this.state.colorString1} onChange={this.onColorChange} style={styles.colorInputs} />
            //                 </div>
            //                 <div style={styles.flexcol}>
            //                     <p style={styles.inputTitle}>density</p>
            //                     <input type="color" id="color2" ref="fgColor1" value={this.state.colorString2} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color3" ref="fgColor2" value={this.state.colorString3} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color4" ref="fgColor3" value={this.state.colorString4} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color5" ref="fgColor4" value={this.state.colorString5} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color6" ref="fgColor5" value={this.state.colorString6} onChange={this.onColorChange} style={styles.colorInputs} />
            //                 </div>
            //                 <div style={styles.flexcol}>
            //                     <p style={styles.inputTitle}>select</p>
            //                     <input type="color" id="color7"  ref="brColor1" value={this.state.colorString7}  onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color8"  ref="brColor2" value={this.state.colorString8}  onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color9"  ref="brColor3" value={this.state.colorString9}  onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color10" ref="brColor4" value={this.state.colorString10} onChange={this.onColorChange} style={styles.colorInputs} />
            //                     <input type="color" id="color11" ref="brColor5" value={this.state.colorString11} onChange={this.onColorChange} style={styles.colorInputs} />
            //                 </div>
            //             </div>
            //         </div>
            //     </div>
            // </div>
            //

