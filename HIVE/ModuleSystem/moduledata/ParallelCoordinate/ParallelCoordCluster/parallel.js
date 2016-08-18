/* ****************************************************************************
 * parentElement === target dom node
 * options[optional] === {
 *     padding         : <number> Canvas 内部に設ける描画領域のパディング
 *     svg: {
 *         defaultwidth: <number> クラスタとして描かれる軸周辺の矩形の幅
 *         textbaseline: <number> 軸名描画領域の高さを決めるためのベースライン
 *         textsize    : <string> 軸タイトルのフォントサイズ
 *         scalesize   : <string> 軸目盛のフォントサイズ
 *     },
 *     axis: {
 *         linewidth   : <number> 軸の線の幅
 *         linecolor   : <string> 軸の色
 *         scalewidth  : <number> 軸の目盛線の横方向への伸び幅
 *     },
 *     bezier: {
 *         division    : <number> ベジェ曲線ポリゴンの分割数
 *         linescale   : <number> ベジェ曲線ポリゴンの厚み（高さ）係数
 *     },
 *     plot: {
 *         width       : <number> Scatter plot エリアの幅（現状固定するので）
 *         color       : <string> 矩形に対して適用する色 CSS 準拠
 *     }
 * }
 * ************************************************************************* */

function ParallelCoordCluster(parentElement, option){
    this.width = 0;
    this.height = 0;
    this.padding = 0;
    this.axisCount = 0;      // 自身に含まれる列の数
    this.axisArray = [];     // 列（Axis インスタンス）格納する配列
    this.beginFlow = 'left'; // どちらが始点となりデータが流れていくか（未使用）
    this.stateData = null;   // 選択状況も含めた内部的なデータ全体

    this.parent = parentElement;                        // 自身を格納している親 DOM
    this.parentElement = document.createElement('div'); // Canvas 側の親 DOM
    this.plotElement = document.createElement('div');   // Scatter plot 用の領域も動的生成
    this.canvas = document.createElement('canvas');     // Canvas は動的に生成
    this.layer = document.createElement('div');         // Canvas の上に乗るレイヤ（SVG などが入る）も動的生成
    this.plotCanvas = document.createElement('canvas');     // Plot 用の Canvas は動的に生成
    this.plotLayer = document.createElement('div');         // Plot 用の Canvas の上に乗るレイヤ（SVG などが入る）も動的生成
    this.parent.appendChild(this.parentElement);        // 親 DOM に Canvas と Layer を格納する DOM を append
    this.parent.appendChild(this.plotElement);          // 親 DOM に plot 用の DOM を append
    this.parentElement.appendChild(this.canvas);        // Canvas を親 DOM 内の外装に append
    this.parentElement.appendChild(this.layer);         // Layer を親 DOM 内の外装に append
    this.plotElement.appendChild(this.plotCanvas);        // Canvas を親 DOM 内の外装に append
    this.plotElement.appendChild(this.plotLayer);         // Layer を親 DOM 内の外装に append

    // callback function
    this.selectedCallback = null;

    this.gl = null;
    this.glReady = false;
    this.glFrame = null;
    this.glFrameSize = 512;
    this.mat = null;
    this.qtn = null;
    this.drawRect = null;

    this.NS_SVG = 'http://www.w3.org/2000/svg';
    this.NS = function(e){return document.createElementNS(this.NS_SVG, e);}.bind(this);

    this.PARALLEL_PADDING = 30;
    this.SVG_DEFAULT_WIDTH = 40;
    this.SVG_TEXT_BASELINE = 30;
    this.SVG_TEXT_SIZE = 'medium';
    this.SVG_SCALE_SIZE = 'small';
    this.AXIS_LINE_WIDTH = 2;
    this.AXIS_LINE_COLOR = '#333';
    this.AXIS_LINE_SELECT_COLOR = '#666';
    this.AXIS_LINE_BRUSH_COLOR = '#f66';
    this.AXIS_BRUSH_HANDLE_COLOR = 'transparent';
    this.AXIS_BRUSHED_EDGE_HEIGHT = 3;
    this.AXIS_SCALE_WIDTH = 5;
    this.BEZIER_DIVISION = 100;
    this.BEZIER_LINE_SCALE = 3.0;
    this.PLOT_AREA_WIDTH = 200;
    this.PLOT_RECT_COLOR = 'deeppink';

    // binding
    this.setOption = this.setOption.bind(this);
    this.checkOption = this.checkOption.bind(this);

    // option setting
    this.setOption(option);

    // style modify
    this.parent.style.display = 'flex';
    this.parent.style.flexDirection = 'row';
    this.parentElement.style.width = 'calc(100% - ' + this.PLOT_AREA_WIDTH + 'px)';
    this.plotElement.style.width = this.PLOT_AREA_WIDTH + 'px';
    this.plotElement.style.height = '100%';
    this.canvas.style.backgroundColor = 'white';
    this.canvas.style.float = 'left';
    this.canvas.width = this.parentElement.clientWidth;
    this.canvas.height = this.parentElement.clientHeight;
    this.canvas.style.position = 'absolute';
    this.layer.style.width = this.parentElement.clientWidth + 'px';
    this.layer.style.height = this.parentElement.clientHeight + 'px';
    this.layer.style.position = 'relative';
    this.plotCanvas.style.float = 'left';
    this.plotCanvas.width = this.plotElement.clientWidth;
    this.plotCanvas.height = this.plotElement.clientHeight;
    this.plotCanvas.style.position = 'absolute';
    this.plotLayer.style.width = this.plotElement.clientWidth + 'px';
    this.plotLayer.style.height = this.plotElement.clientHeight + 'px';
    this.plotLayer.style.position = 'relative';

    this.initCanvas();                  // canvas の WebGL 関連初期化
    this.resetCanvas();                 // 初期化以降にリセットする場合
    this.resetBezierCanvas();           // ベジェ曲線モードでリセット
    this.resetBezierGeometryCanvas();   // ベジェ曲線ポリゴンジオメトリモード
    this.drawRect = this.getDrawRect(); // 描画対象の矩形領域
    this.mouseX = 0;
    this.mouseY = 0;
    this.mouseNormalX = 0;
    this.mouseNormalY = 0;
    this.selectedArray = []; // brush されたときの情報
}
// オプションをセットする
ParallelCoordCluster.prototype.setOption = function(option){
    if(!option){return false;}
    var s = 'padding';
    if(this.checkOption(option, s)){this.PARALLEL_PADDING = option[s];}
    if(this.checkOption(option, 'svg')){
        s = 'defaultwidth';
        if(this.checkOption(option.svg, s)){this.SVG_DEFAULT_WIDTH = option.svg[s];}
        s = 'textbaseline';
        if(this.checkOption(option.svg, s)){this.SVG_TEXT_BASELINE = option.svg[s];}
        s = 'textsize';
        if(this.checkOption(option.svg, s)){this.SVG_TEXT_SIZE = option.svg[s];}
        s = 'scalesize';
        if(this.checkOption(option.svg, s)){this.SVG_SCALE_SIZE = option.svg[s];}
    }
    if(this.checkOption(option, 'axis')){
        s = 'linewidth';
        if(this.checkOption(option.axis, s)){this.AXIS_LINE_WIDTH = option.axis[s];}
        s = 'linecolor';
        if(this.checkOption(option.axis, s)){this.AXIS_LINE_COLOR = option.axis[s];}
        s = 'scalewidth';
        if(this.checkOption(option.axis, s)){this.AXIS_SCALE_WIDTH = option.axis[s];}
    }
    if(this.checkOption(option, 'bezier')){
        s = 'division';
        if(this.checkOption(option.bezier, s)){this.BEZIER_DIVISION = option.bezier[s];}
        s = 'linescale';
        if(this.checkOption(option.bezier, s)){this.BEZIER_LINE_SCALE = option.bezier[s];}
    }
    if(this.checkOption(option, 'plot')){
        s = 'width';
        if(this.checkOption(option.bezier, s)){this.PLOT_AREA_WIDTH = option.plot[s];}
        s = 'color';
        if(this.checkOption(option.bezier, s)){this.PLOT_RECT_COLOR = option.plot[s];}
    }
    if(this.checkOption(option, 'callback')){
        s = 'selected';
        if(this.checkOption(option.callback, s)){this.selectedCallback = option.callback[s];}
    }
};
ParallelCoordCluster.prototype.checkOption = function(option, name){
    return (
        option.hasOwnProperty(name) &&
        option[name] !== null &&
        option[name] !== undefined &&
        option[name] !== '' &&
        option[name] !== 0
    );
};
// コンポーネントのサイズをセット
ParallelCoordCluster.prototype.setRect = function(width, height){
    this.canvas.width = this.parentElement.clientWidth;
    this.canvas.height = this.parentElement.clientHeight;
    this.layer.style.width = this.parentElement.clientWidth + 'px';
    this.layer.style.height = this.parentElement.clientHeight + 'px';
    this.plotCanvas.width = this.plotElement.clientWidth;
    this.plotCanvas.height = this.plotElement.clientHeight;
    this.plotLayer.style.width = this.plotElement.clientWidth + 'px';
    this.plotLayer.style.height = this.plotElement.clientHeight + 'px';
};
// 列追加
ParallelCoordCluster.prototype.addAxis = function(){
    var i, j;
    for(i = 0, j = this.stateData.axis.length; i < j; ++i){
        this.axisArray.push(new Axis(this, i));
        this.axisCount = this.axisArray.length;
    }
};
// 列の配置をリセットして可能なら canvas を再描画する
// resetData is optional argument
ParallelCoordCluster.prototype.resetAxis = function(resetData){
    var i, j;
    var space, margin;
    if(resetData && resetData.hasOwnProperty('axis') && resetData.axis.length > 0){
        for(i = 0; i < this.axisCount; ++i){
            this.axisArray[i].delete();
        }
        // reset state and add data
        this.stateData = resetData;
        this.selectedArray = [];
        this.axisArray = [];
        this.addAxis();
    }
    space = this.layer.clientWidth - this.PARALLEL_PADDING * 2;
    margin = space / (this.axisCount - 1);
    for(i = 0; i < this.axisCount; ++i){
        this.axisArray[i].update();
    }
    for(i = 0; i < this.axisCount; ++i){
        j = this.PARALLEL_PADDING + (margin - this.SVG_DEFAULT_WIDTH) * i - this.SVG_DEFAULT_WIDTH / 2;
        this.axisArray[i].setPosition(j);
    }
    if(this.glReady){
        this.drawCanvas();
    }
    return this;
};
// WebGL コンテキストなどの初期化
ParallelCoordCluster.prototype.initCanvas = function(){
    this.gl = this.canvas.getContext('webgl');
    this.glReady = this.gl !== null && this.gl !== undefined;
    this.glFrameSize = 1024;
    this.glFrame = create_framebuffer(this.gl, null, this.glFrameSize, this.glFrameSize);
    this.layer.addEventListener('mousemove', (function(eve){
        var r = eve.currentTarget.getBoundingClientRect();
        var x = eve.clientX - r.left;
        var y = eve.clientY - r.top;
        var topMargin = this.PARALLEL_PADDING + this.SVG_TEXT_BASELINE;
        this.mouseX = x;
        this.mouseY = y;
        this.mouseNormalX = Math.min(Math.max(0, x - this.drawRect.x), this.drawRect.width) / this.drawRect.width;
        this.mouseNormalY = Math.min(Math.max(0, (r.height - y) - this.drawRect.y), this.drawRect.height) / this.drawRect.height;
        if(this.glReady){
            var gl = this.gl;
            var u8 = new Uint8Array(4);
            gl.bindFramebuffer(gl.FRAMEBUFFER, this.glFrame.framebuffer);
            gl.readPixels(x, r.height - y, 1, 1, gl.RGBA, gl.UNSIGNED_BYTE, u8);
        }
    }).bind(this), false);
    return this;
};
// canvas に矩形とか描けるやーつ
ParallelCoordCluster.prototype.resetCanvas = function(){
    var gl = this.gl;
    var mat = this.mat;
    if(!this.glReady){return;}
    if(!this.mat){this.mat = new matIV();}
    if(!this.qtn){this.qtn = new qtnIV();}
    this.drawRect = this.getDrawRect();

    var vSource = '';
    vSource += 'attribute vec3 position;';
    vSource += 'uniform mat4 matrix;';
    vSource += 'void main(){';
    vSource += '    gl_Position = matrix * vec4(position, 1.0);';
    vSource += '}';
    var fSource = '';
    fSource += 'precision mediump float;';
    fSource += 'uniform vec4 color;';
    fSource += 'void main(){';
    fSource += '    gl_FragColor = color;';
    fSource += '}';
    var vs = create_shader(gl, vSource, gl.VERTEX_SHADER);
    var fs = create_shader(gl, fSource, gl.FRAGMENT_SHADER);
    this.prg = create_program(gl, vs, fs);
    this.attL = [gl.getAttribLocation(this.prg, 'position')];
    this.attS = [3];
    this.uniL = {
        matrix: gl.getUniformLocation(this.prg, 'matrix'),
        color:  gl.getUniformLocation(this.prg, 'color')
    };
    var position = [
        0.5, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0 // triangle look
    ];
    var vPosition = create_vbo(gl, position);
    this.vboList = [vPosition];
};
// ベジェ曲線をラインで描く
ParallelCoordCluster.prototype.resetBezierCanvas = function(){
    var i, j;
    var gl = this.gl;
    var mat = this.mat;
    if(!this.glReady){return;}
    if(!this.mat){this.mat = new matIV();}
    if(!this.qtn){this.qtn = new qtnIV();}
    this.drawRect = this.getDrawRect();

    var vSource = '';
    vSource += 'attribute vec3 position;';
    vSource += 'uniform mat4 matrix;';
    vSource += 'uniform vec4 point;';
    vSource += '';
    vSource += 'vec2 bezier(float t, vec2 p0, vec2 p1, vec2 p2, vec2 p3){';
    vSource += '    float r = 1.0 - t;';
    vSource += '    return vec2(  r * r * r *     p0.x +';
    vSource += '                3.0 * r * r * t * p1.x +';
    vSource += '                3.0 * r * t * t * p2.x +';
    vSource += '                  t * t * t *     p3.x,';
    vSource += '                  r * r * r *     p0.y +';
    vSource += '                3.0 * r * r * t * p1.y +';
    vSource += '                3.0 * r * t * t * p2.y +';
    vSource += '                  t * t * t *     p3.y);';
    vSource += '}';
    vSource += '';
    vSource += 'void main(){';
    vSource += '    vec2 p0 = vec2(point.x, point.z);';
    vSource += '    vec2 p1 = vec2(point.x + (point.y - point.x) * 0.5, point.z);';
    vSource += '    vec2 p2 = vec2(point.y + (point.x - point.y) * 0.5, point.w);';
    vSource += '    vec2 p3 = vec2(point.y, point.w);';
    vSource += '    gl_Position = matrix * vec4(bezier(position.x, p0, p1, p2, p3), 0.0, 1.0);';
    vSource += '}';
    var fSource = '';
    fSource += 'precision mediump float;';
    fSource += 'uniform vec4 color;';
    fSource += 'void main(){';
    fSource += '    gl_FragColor = color;';
    fSource += '}';
    var vs = create_shader(gl, vSource, gl.VERTEX_SHADER);
    var fs = create_shader(gl, fSource, gl.FRAGMENT_SHADER);
    this.bPrg = create_program(gl, vs, fs);
    this.bAttL = [gl.getAttribLocation(this.bPrg, 'position')];
    this.bAttS = [3];
    this.bUniL = {
        matrix: gl.getUniformLocation(this.bPrg, 'matrix'),
        point:  gl.getUniformLocation(this.bPrg, 'point'),
        color:  gl.getUniformLocation(this.bPrg, 'color')
    };
    var position = [];
    j = 1.0 / this.BEZIER_DIVISION;
    j += j / this.BEZIER_DIVISION;
    for(i = 0; i < this.BEZIER_DIVISION; ++i){
        position.push(i * j, 0.0, 0.0);
    }
    var vPosition = create_vbo(gl, position);
    this.bVboList = [vPosition];
    return this;
};
// ベジェ曲線をジオメトリとして描く
ParallelCoordCluster.prototype.resetBezierGeometryCanvas = function(){
    var i, j;
    var gl = this.gl;
    var mat = this.mat;
    if(!this.glReady){return;}
    if(!this.mat){this.mat = new matIV();}
    if(!this.qtn){this.qtn = new qtnIV();}
    this.drawRect = this.getDrawRect();

    var vSource = '';
    vSource += 'attribute vec3 position;';
    vSource += 'attribute float signs;';
    vSource += 'uniform mat4 matrix;';
    vSource += 'uniform vec4 point;';
    vSource += 'uniform float nextTime;';
    vSource += 'uniform float scale;';
    vSource += 'const float PI = 3.141592;';
    vSource += '';
    vSource += 'vec2 bezier(float t, vec2 p0, vec2 p1, vec2 p2, vec2 p3){';
    vSource += '    float r = 1.0 - t;';
    vSource += '    return vec2(  r * r * r *     p0.x +';
    vSource += '                3.0 * r * r * t * p1.x +';
    vSource += '                3.0 * r * t * t * p2.x +';
    vSource += '                  t * t * t *     p3.x,';
    vSource += '                  r * r * r *     p0.y +';
    vSource += '                3.0 * r * r * t * p1.y +';
    vSource += '                3.0 * r * t * t * p2.y +';
    vSource += '                  t * t * t *     p3.y);';
    vSource += '}';
    vSource += '';
    vSource += 'void main(){';
    vSource += '    float f = abs(position.x - 0.5) * 2.0;';
    vSource += '    float g = (1.0 - cos(f * PI)) * 0.75 + 1.0;';
    vSource += '    vec2 p0 = vec2(point.x, point.z);';
    vSource += '    vec2 p1 = vec2(point.x + (point.y - point.x) * 0.5, point.z);';
    vSource += '    vec2 p2 = vec2(point.y + (point.x - point.y) * 0.5, point.w);';
    vSource += '    vec2 p3 = vec2(point.y, point.w);';
    vSource += '    vec2 p  = bezier(position.x, p0, p1, p2, p3);';
    vSource += '    vec2 n  = bezier(position.x + nextTime, p0, p1, p2, p3);';
    vSource += '    vec2 r  = normalize(n - p);';
    vSource += '    gl_Position = matrix * vec4(p + (vec2(r.y, -r.x) * signs) * scale * g, 0.0, 1.0);';
    vSource += '}';
    var fSource = '';
    fSource += 'precision mediump float;';
    fSource += 'uniform vec4 color;';
    fSource += 'void main(){';
    fSource += '    gl_FragColor = color;';
    fSource += '}';
    var vs = create_shader(gl, vSource, gl.VERTEX_SHADER);
    var fs = create_shader(gl, fSource, gl.FRAGMENT_SHADER);
    this.bgPrg = create_program(gl, vs, fs);
    this.bgAttL = [
        gl.getAttribLocation(this.bgPrg, 'position'),
        gl.getAttribLocation(this.bgPrg, 'signs')
    ];
    this.bgAttS = [3, 1];
    this.bgUniL = {
        matrix:   gl.getUniformLocation(this.bgPrg, 'matrix'),
        point:    gl.getUniformLocation(this.bgPrg, 'point'),
        nextTime: gl.getUniformLocation(this.bgPrg, 'nextTime'),
        scale:    gl.getUniformLocation(this.bgPrg, 'scale'),
        color:    gl.getUniformLocation(this.bgPrg, 'color')
    };
    var position = [];
    var signs = [];
    j = 1.0 / this.BEZIER_DIVISION;
    j += j / this.BEZIER_DIVISION;
    for(i = 0; i < this.BEZIER_DIVISION; ++i){
        position.push(i * j, 0.0, 0.0, i * j, 0.0, 0.0);
        signs.push(1.0, -1.0);
    }
    var vPosition = create_vbo(gl, position);
    var vSigns = create_vbo(gl, signs);
    this.bgVboList = [vPosition, vSigns];
    return this;
};
// 初期化などが全て終わっている前提の描画実行部分
ParallelCoordCluster.prototype.drawCanvas = function(){
    var a, b, c, d, e, f, i, j, k, l, m, n, o;
    var p, q, r, s, t, u, v, w, x, y;
    var gl = this.gl;
    var mat = this.mat;
    var mMatrix   = mat.identity(mat.create());
    var vMatrix   = mat.identity(mat.create());
    var pMatrix   = mat.identity(mat.create());
    var vpMatrix  = mat.identity(mat.create());
    var mvpMatrix = mat.identity(mat.create());
    if(!this.glReady){return;}
    this.canvas.width = this.parentElement.clientWidth;
    this.canvas.height = this.parentElement.clientHeight;
    this.drawRect = this.getDrawRect();
    mat.lookAt(
        [0.0, 0.0, 1.0],
        [0.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        vMatrix
    );
    mat.ortho(
        0,
        this.drawRect.width,
        this.drawRect.height,
        0,
        0.5,
        1.0,
        pMatrix
    );
    mat.multiply(pMatrix, vMatrix, vpMatrix);

    var drawClusterRect = function(left, right, top, bottom, color, summit){
        gl.disable(gl.BLEND);
        var w = right - left;
        var h = top - bottom;
        // mat.identity(mMatrix);
        // mat.translate(mMatrix, [left - w / 2, bottom, 0.0], mMatrix);
        // mat.scale(mMatrix, [w, h, 1.0], mMatrix);
        // mat.multiply(vpMatrix, mMatrix, mvpMatrix);
        //
        // gl.useProgram(this.prg);
        // gl.uniformMatrix4fv(this.uniL.matrix, false, mvpMatrix);
        // gl.uniform4fv(this.uniL.color, color);
        //
        // set_attribute(gl, this.vboList, this.attL, this.attS);
        // gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

        // top triangle ==========
        // debugger;
        var nSummit = summit;
        mat.identity(mMatrix);
        mat.translate(mMatrix, [0.0, (h * (1.0 - nSummit)), 0.0], mMatrix);
        mat.translate(mMatrix, [left - w / 2, bottom, 0.0], mMatrix);
        mat.scale(mMatrix, [w, h * nSummit, 1.0], mMatrix);
        mat.multiply(vpMatrix, mMatrix, mvpMatrix);
        gl.useProgram(this.prg);
        gl.uniformMatrix4fv(this.uniL.matrix, false, mvpMatrix);
        gl.uniform4fv(this.uniL.color, color);
        set_attribute(gl, this.vboList, this.attL, this.attS);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        // bottom triangle ========
        mat.identity(mMatrix);
        mat.translate(mMatrix, [0.0, (h * (1.0 - nSummit)), 0.0], mMatrix);
        mat.translate(mMatrix, [left + w / 2, bottom, 0.0], mMatrix);
        mat.rotate(mMatrix, Math.PI, [0.0, 0.0, 1.0], mMatrix);
        mat.scale(mMatrix, [w, h * (1.0 - nSummit), 1.0], mMatrix);
        mat.multiply(vpMatrix, mMatrix, mvpMatrix);
        gl.useProgram(this.prg);
        gl.uniformMatrix4fv(this.uniL.matrix, false, mvpMatrix);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
    }.bind(this);

    var drawBeziercurve = function(left, right, first, second, color){
        gl.enable(gl.BLEND);
        gl.blendFuncSeparate(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA, gl.ONE, gl.ONE);
        gl.useProgram(this.bPrg);
        gl.uniformMatrix4fv(this.bUniL.matrix, false, vpMatrix);
        gl.uniform4fv(this.bUniL.point, [left, right, first, second]);
        gl.uniform4fv(this.bUniL.color, color);

        set_attribute(gl, this.bVboList, this.bAttL, this.bAttS);
        gl.drawArrays(gl.LINE_STRIP, 0, this.BEZIER_DIVISION);
    }.bind(this);

    var drawBezierGeometry = function(left, right, first, second, color){
        gl.disable(gl.BLEND);
        gl.useProgram(this.bgPrg);
        gl.uniformMatrix4fv(this.bgUniL.matrix, false, vpMatrix);
        gl.uniform4fv(this.bgUniL.point, [left, right, first, second]);
        gl.uniform1f(this.bgUniL.nextTime, 1.0 / this.BEZIER_DIVISION);
        gl.uniform1f(this.bgUniL.scale, this.BEZIER_LINE_SCALE);
        gl.uniform4fv(this.bgUniL.color, color);

        set_attribute(gl, this.bgVboList, this.bgAttL, this.bgAttS);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, this.BEZIER_DIVISION * 2);
    }.bind(this);

    if(this.axisArray.length > 1){
        a = [];
        for(i = 0, j = this.axisArray.length; i < j; ++i){
            a[i] = {cluster: null, line: []};
            // cluster sort
            b = [];
            d = [];
            for(k = 0, l = this.axisArray[i].clusters.length; k < l; ++k){
                b.push({index: k, value: this.axisArray[i].clusters[k].color[3]});
                if(i !== this.axisArray.length - 1){
                    // line sort
                    c = this.axisArray[i].clusters[k].getOutputLinePower();
                    for(m = 0, n = c.length; m < n; ++m){
                        d[m] = {index: m, value: c[m]};
                    }
                    d.sort(function(a, b){return a.value - b.value;});
                    a[i].line[k] = d.concat();
                }
            }
            b.sort(function(a, b){return a.value - b.value;});
            a[i].cluster = b.concat();
        }
        render.bind(this)(this.glFrame.framebuffer);
        render.bind(this)(null);
        function render(target){
            var gl = this.gl;
            gl.bindFramebuffer(gl.FRAMEBUFFER, target);
            if(target){
                gl.viewport(this.drawRect.x, this.drawRect.y, this.drawRect.width, this.drawRect.height);
                gl.clearColor(0.0, 0.0, 0.0, 0.0);
                gl.clear(gl.COLOR_BUFFER_BIT);
            }else{
                gl.viewport(this.drawRect.x, this.drawRect.y, this.drawRect.width, this.drawRect.height);
                gl.clearColor(1.0, 1.0, 1.0, 1.0);
                gl.clear(gl.COLOR_BUFFER_BIT);
            }
            for(i = 0, j = this.axisArray.length; i < j; ++i){
                q = this.axisArray[i].height - this.SVG_TEXT_BASELINE;     // Canvas の描画すべきエリアの高さ
                x = this.axisArray[i].getHorizontalRange();                // 対象軸の X 座標（非正規）
                for(e = 0, f = this.axisArray[i].clusters.length; e < f; ++e){
                    k = a[i].cluster[e].index;                             // ソート済みの描画すべきインデックス
                    v = this.axisArray[i].clusters[k].getNormalizeRange(); // クラスタの上下限値（正規）
                    w = q * v.min;                                         // 高さに正規化済みのクラスタの下限値掛ける
                    y = q * v.max;                                         // 高さに正規化済みのクラスタの上限値掛ける
                    // bezier curve
                    if(i !== (this.axisArray.length - 1)){                 // 最終軸じゃないときだけやる
                        t = this.axisArray[i + 1].getHorizontalRange();    // 右隣の軸の X 座標（非正規）
                        u = (y - w) * v.top + w;                           // v.top は正規化されているので除算ではなく乗算
                        var linePower = this.axisArray[i].clusters[k].getOutputLinePower();
                        for(o = 0, s = this.axisArray[i + 1].clusters.length; o < s; ++o){
                            r = a[i].line[k][o].index;                     // ソート済みの描画すべきインデックス
                            v = this.axisArray[i + 1].clusters[r].getNormalizeRange();
                            w = q * ((v.max - v.min) * v.top + v.min);     // v.top は正規化されているので除算ではなく乗算する
                            p = 0.5;
                            // x == 対称軸の X 座標
                            // t == 右軸の X 座標
                            // u == 対象クラスタの中心の Y 座標
                            // w == 右軸対象クラスタの中心の Y 座標
                            var drawColor = this.axisArray[i].clusters[k].color;
                            m = linePower[r] * 0.9 + 0.1;
                            // drawBeziercurve(x, t, u, w, [drawColor[0], drawColor[1], drawColor[2], m]); // ラインで描く場合
                            drawBezierGeometry(x, t, u, w, [drawColor[0], drawColor[1], drawColor[2], m]); // モノクロgeometry
                        }
                    }
                }
            }
            // 少々冗長なのだが、軸上の矩形を確実に上に描画させるために
            for(i = 0, j = this.axisArray.length; i < j; ++i){
                q = this.axisArray[i].height - this.SVG_TEXT_BASELINE;     // Canvas の描画すべきエリアの高さ
                x = this.axisArray[i].getHorizontalRange();                // 対象軸の X 座標（非正規）
                for(e = 0, f = this.axisArray[i].clusters.length; e < f; ++e){
                    k = a[i].cluster[e].index;                             // ソート済みの描画すべきインデックス
                    v = this.axisArray[i].clusters[k].getNormalizeRange(); // クラスタの上下限値（正規）
                    w = q * v.min;                                         // 高さに正規化済みのクラスタの下限値掛ける
                    y = q * v.max;                                         // 高さに正規化済みのクラスタの上限値掛ける
                    var _min = this.axisArray[i].clusters[k].min;
                    var _max = this.axisArray[i].clusters[k].max;
                    var _top = this.axisArray[i].clusters[k].top;
                    drawClusterRect(
                        x,
                        x + this.SVG_DEFAULT_WIDTH,
                        y,
                        w,
                        this.axisArray[i].clusters[k].color, // モノクロ
                        (_max - _top) / (_max - _min)
                    );
                }
            }
        }
    }

    gl.flush();

    return this;
};
// 描画対象となる矩形を得る
ParallelCoordCluster.prototype.getDrawRect = function(){
    var w = this.parentElement.clientWidth - this.PARALLEL_PADDING * 2;
    var h = this.parentElement.clientHeight - this.PARALLEL_PADDING * 2 - this.SVG_TEXT_BASELINE;
    return {
        x: this.PARALLEL_PADDING,
        y: this.PARALLEL_PADDING,
        width: w,
        height: h,
        mx: this.mouseX,
        my: this.mouseY,
        mnx: this.mouseNormalX,
        mny: this.mouseNormalY
    };
};
// ステートをデータ構造として返す
ParallelCoordCluster.prototype.getStateData = function(){
    return this.stateData;
};
// ステートを JSON で返す
ParallelCoordCluster.prototype.getStateJSON = function(){
    return JSON.stringify(this.stateData);
};
// 全軸上のその時点での選択範囲・input に返すべき情報をJSONにして返す
ParallelCoordCluster.prototype.getAllBrushedRange = function(currentAxis){
    var f, e, i, j, k, l, v, w;
    var min, max, len;
    var selLength = this.selectedArray.length;

    // return value gen and update
    v = []; w = []; e = [];
    for(i = 0, j = this.axisArray.length; i < j; ++i){
        min = max = len = null;
        v[i] = this.axisArray[i].getBrushedRange();
        e[i] = (v[i] && v[i].top && v[i].bottom);
        if(e[i]){
            len = this.axisArray[i].max - this.axisArray[i].min;
            min = len * (1.0 - v[i].bottom) + this.axisArray[i].min;
            max = len * (1.0 - v[i].top)    + this.axisArray[i].min;
        }
        this.stateData.axis[i].brush.min = min; // brush 領域の min
        this.stateData.axis[i].brush.max = max; // brush 領域の max
        this.stateData.axis[i].volume = {       // ボリューム全体の軸ごとの minmax
            min: this.stateData.volume.minmax[i].min,
            max: this.stateData.volume.minmax[i].max
        };
        w[i] = this.axisArray[i].getClusterBrushed();
        for(k = 0, l = w[i].length; k < l; ++k){
            if(w[i][k]){
                this.stateData.axis[i].cluster[k].selected = w[i][k];
            }else{
                this.stateData.axis[i].cluster[k].selected = false;
            }
        }
    }

    // check selection count
    f = false;
    j = currentAxis.index;
    for(i = 0; i < selLength; ++i){                 // 既存の選択状態をチェックする
        if(this.selectedArray[i].index === j){      // インデックスが一致しているか
            f = true;                               // 一致していたものが存在した
            if(e[j]){                               // 選択されているか
                // 一致していたものが存在し更新
                this.selectedArray[i].top = v[j].top;
                this.selectedArray[i].bottom = v[j].bottom;
            }else{
                // 一致したが選択されていない状態なので消す
                this.selectedArray.splice(i, 1);
            }
            break;
        }
    }
    // 一致していたものが存在しなかったら追加する
    if(!f){
        this.selectedArray.push({
            index: currentAxis.index,
            top: v[j].top,
            bottom: v[j].bottom
        });
    }

    this.setClusterColor(e);
    return this.stateData.axis;
};
ParallelCoordCluster.prototype.setClusterColor = function(selectedAxis){
    var i, j, k, l, m, n;
    var a;
    var colorStride = 360 / 2.5;

    // いったん色を全リセット
    for(i = 0, j = this.axisArray.length; i < j; ++i){
        for(k = 0, l = this.axisArray[i].clusters.length; k < l; ++k){
            this.axisArray[i].clusters[k].color[0]     = 0.0;
            this.axisArray[i].clusters[k].color[1]     = 0.0;
            this.axisArray[i].clusters[k].color[2]     = 0.0;
            this.stateData.axis[i].cluster[k].color[0] = 0.0;
            this.stateData.axis[i].cluster[k].color[1] = 0.0;
            this.stateData.axis[i].cluster[k].color[2] = 0.0;
        }
    }

    // selected array で選択されているものを順番に影響力求める
    for(i = 0, j = this.selectedArray.length; i < j; ++i){
        m = this.selectedArray[i].index; // 最初に選択されたものから順に
        n = 0;
        for(k = 0, l = this.axisArray[m].clusters.length; k < l; ++k){
            if(this.stateData.axis[m].cluster[k].selected){
                // まずは自分自身の色を決める
                a = hsva(colorStride * n, 1.0, 0.5, 1.0);
                this.axisArray[m].clusters[k].color[0]     = a[0];
                this.axisArray[m].clusters[k].color[1]     = a[1];
                this.axisArray[m].clusters[k].color[2]     = a[2];
                this.stateData.axis[m].cluster[k].color[0] = a[0];
                this.stateData.axis[m].cluster[k].color[1] = a[1];
                this.stateData.axis[m].cluster[k].color[2] = a[2];
                ++n;
            }
        }
    }
};

// axis ===================================================================
function Axis(parent, index){
    var i, j;
    var axisData = parent.stateData.axis[index];
    this.parent = parent;                // 親となる ParallelCoordCluster インスタンス
    this.title = axisData.title;         // 列のラベル
    this.index = index;                  // インデックス（通常左から読み込んだ順に配置）
    this.svg = this.parent.NS('svg');    // SVG エレメント
    this.axisRectSvg = null;             // axis area rect
    this.brushed = false;                // brushing flag
    this.onBrush = false;                // on brush start flag
    this.onBrushRect = false;            // on brush rect drag start flag
    this.eventCurrentSvg = null;         // use currentTarget check
    this.brushDefaultHeight = 0;         // brush start height (normalize range)
    this.brushStartHeight = 0;           // brush start height (normalize range)
    this.brushEndHeight = 0;             // brush end height (normalize range)
    this.brushRectSvg = null;            // brush area
    this.brushTopRectSvg = null;         // brush top area
    this.brushBottomRectSvg = null;      // brush bottom area
    this.brushRectDefaultHeight = 0;     // brush start height (normalize range)
    this.min = 0;                        // min
    this.max = 0;                        // max
    this.width = 0;
    this.height = 0;
    this.left = 0;
    this.onAxisTitleDrag = false;     // ドラッグされているかどうかのフラグ
    this.centerH = 0;        // 軸の中心が矩形の左から何ピクセル目にあるか
    this.bbox = null;        // svg.getBBox の結果
    this.listeners = [];     // リスナを殺すためにキャッシュするので配列を用意
    this.clusters = [];      // 自身に格納しているクラスタ
    this.putData = {left: null, right: null};
    this.dataLength = parent.stateData.edge.volumenum;
    if(index === 0){
        this.putData.right = parent.stateData.edge.cluster[index];
    }else if(index === parent.stateData.edge.cluster.length){
        this.putData.left  = parent.stateData.edge.cluster[index - 1];
    }else{
        this.putData.left  = parent.stateData.edge.cluster[index - 1];
        this.putData.right = parent.stateData.edge.cluster[index];
    }
    for(i = 0, j = axisData.cluster.length; i < j; ++i){
        this.clusters.push(new Cluster(
            this, // axis 自身
            i,    // axis のインデックス
            axisData.cluster[i].selected, // 選択状態
            axisData.cluster[i].top,      // temp ※アウトの仕様がまだ未確定なので枠のみnullにならないようにそのままにしておく
            // axisData.cluster[i].out, // クラスタ自身からの出力
            axisData.cluster[i].min,      // min
            axisData.cluster[i].max,      // max
            axisData.cluster[i].top,      // top
            null                          // ここは将来的に色が入る可能性がある
        ));
        this.parent.stateData.axis[index].cluster[i].color[0] = this.clusters[i].color[0];
        this.parent.stateData.axis[index].cluster[i].color[1] = this.clusters[i].color[1];
        this.parent.stateData.axis[index].cluster[i].color[2] = this.clusters[i].color[2];
        this.parent.stateData.axis[index].cluster[i].color[3] = this.clusters[i].color[3];
    }
    this.getClustersMinMax();    // クラスタの minmax とってきて自身に適用
    this.svg.style.position = 'relative';
    this.svg.style.overflow = 'visible';
    this.parent.layer.appendChild(this.svg);
}
// 軸を設定して SVG を生成して描画する
Axis.prototype.update = function(titleString, minmax){
    var i, j;
    var path = null;
    var text = null;
    var title = titleString;
    var funcDown = this.dragStart.bind(this);
    var funcMove = this.dragMove.bind(this);
    var funcUp   = this.dragEnd.bind(this);
    var funcAxisDown = this.dragAxisStart.bind(this);
    var funcAxisMove = this.dragAxisMove.bind(this);
    var funcAxisUp   = this.dragAxisEnd.bind(this);
    var funcAxisHandle = this.dragAxisHandleStart.bind(this);
    var funcBrushDown = this.dragAxisBrushStart.bind(this);
    var funcBrushMove = this.dragAxisBrushMove.bind(this);
    var funcBrushUp   = this.dragAxisBrushEnd.bind(this);
    if(titleString){
        this.title = titleString;
    }else{
        title = this.title;
    }
    if(minmax && minmax.hasOwnProperty('length') && minmax.length > 0){
        this.setMinMax(minmax[0], minmax[1]);
    }
    // 軸上のイベントがメモリリークしないように
    if(this.listeners.length > 0){
        for(i = 0, j = this.listeners.length; i < j; ++i){
            this.listeners[i].bind(this)();
        }
        this.listeners = [];
    }
    this.svg.innerHTML = '';
    text = this.parent.NS('text');
    text.addEventListener('mousedown', funcDown, false);
    this.parent.layer.addEventListener('mousemove', funcMove, false);
    this.parent.layer.addEventListener('mouseup', funcUp, false);
    this.listeners.push(
        (function(){return function(){text.removeEventListener('mousedown', funcDown, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mousemove', funcMove, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mouseup', funcUp, false);};}())
    );
    text.textContent = title;
    text.style.fontSize = this.parent.SVG_TEXT_SIZE;
    text.setAttribute('color', this.parent.AXIS_LINE_COLOR);
    text.setAttribute('x', 0);
    text.setAttribute('y', this.parent.SVG_TEXT_BASELINE - 10);
    text.style.cursor = 'pointer';
    this.svg.appendChild(text);
    this.bbox = text.getBBox();
    this.width = this.bbox.width;
    this.height = this.parent.layer.clientHeight - this.parent.PARALLEL_PADDING * 2;
    this.centerH = this.parent.SVG_DEFAULT_WIDTH / 2;
    text.setAttribute('x', -(this.width - this.parent.SVG_DEFAULT_WIDTH) / 2);
    this.svg.style.position = 'relative';
    this.svg.style.width = this.parent.SVG_DEFAULT_WIDTH;
    this.svg.style.height = this.height;
    this.svg.style.top = this.parent.PARALLEL_PADDING;
    this.svg.style.left = this.parent.PARALLEL_PADDING - (this.parent.SVG_DEFAULT_WIDTH / 2);
    path = this.parent.NS('path');
    path.setAttribute('stroke', this.parent.AXIS_LINE_COLOR);
    path.setAttribute('stroke-width', this.parent.AXIS_LINE_WIDTH);
    path.setAttribute(
        'd',
        'M ' + this.centerH + ' ' + this.parent.SVG_TEXT_BASELINE + ' v ' + (this.height - this.parent.SVG_TEXT_BASELINE)
    );
    this.svg.appendChild(path);
    this.drawScale();
    // 軸上の選択可能な領域
    this.axisRectSvg = this.parent.NS('path');
    this.axisRectSvg.setAttribute('fill', 'transparent');
    this.axisRectSvg.setAttribute('stroke', this.parent.AXIS_LINE_SELECT_COLOR);
    this.axisRectSvg.setAttribute('stroke-width', this.parent.AXIS_LINE_WIDTH - 1);
    this.axisRectSvg.setAttribute('class', 'parallelAxisRect');
    this.svg.appendChild(this.axisRectSvg);
    // 軸上の選択された部分を表す矩形
    this.brushRectSvg = this.parent.NS('path');
    this.brushRectSvg.setAttribute('fill', 'rgba(196, 196, 196, 0.5)');
    this.brushRectSvg.setAttribute('stroke', this.parent.AXIS_LINE_BRUSH_COLOR);
    this.brushRectSvg.setAttribute('stroke-width', this.parent.AXIS_LINE_WIDTH - 1);
    this.brushRectSvg.setAttribute('style', 'display: none;');
    this.svg.appendChild(this.brushRectSvg);
    // 軸上の選択領域の上下の先端部分（不可視だがBrush領域を拡縮するのに使う）※上端
    this.brushTopRectSvg = this.parent.NS('path');
    // this.brushTopRectSvg.setAttribute('fill', 'transparent');
    this.brushTopRectSvg.setAttribute('fill', this.parent.AXIS_BRUSH_HANDLE_COLOR);
    this.brushTopRectSvg.setAttribute('stroke', 'transparent');
    this.brushTopRectSvg.setAttribute('style', 'cursor: row-resize; display: none;');
    this.svg.appendChild(this.brushTopRectSvg);
    // 軸上の選択領域の上下の先端部分（不可視だがBrush領域を拡縮するのに使う）※下端
    this.brushBottomRectSvg = this.parent.NS('path');
    // this.brushBottomRectSvg.setAttribute('fill', 'transparent');
    this.brushBottomRectSvg.setAttribute('fill', this.parent.AXIS_BRUSH_HANDLE_COLOR);
    this.brushBottomRectSvg.setAttribute('stroke', 'transparent');
    this.brushBottomRectSvg.setAttribute('style', 'cursor: row-resize; display: none;');
    this.svg.appendChild(this.brushBottomRectSvg);

    // 軸関連のイベントの登録とリムーバの配列へのプッシュ
    this.axisRectSvg.addEventListener('mousedown', funcAxisDown, false);
    this.parent.layer.addEventListener('mousemove', funcAxisMove, false);
    this.parent.layer.addEventListener('mouseup', funcAxisUp, false);
    this.brushTopRectSvg.addEventListener('mousedown', funcAxisHandle, false);
    this.brushBottomRectSvg.addEventListener('mousedown', funcAxisHandle, false);
    this.brushRectSvg.addEventListener('mousedown', funcBrushDown, false);
    this.parent.layer.addEventListener('mousemove', funcBrushMove, false);
    this.parent.layer.addEventListener('mouseup', funcBrushUp, false);
    this.listeners.push(
        (function(){return function(){this.axisRectSvg.removeEventListener('mousedown', funcAxisDown, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mousemove', funcAxisMove, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mouseup', funcAxisUp, false);};}()),
        (function(){return function(){this.brushTopRectSvg.removeEventListener('mousedown', funcAxisHandle, false);};}()),
        (function(){return function(){this.brushBottomRectSvg.removeEventListener('mousedown', funcAxisHandle, false);};}()),
        (function(){return function(){this.brushRectSvg.removeEventListener('mousedown', funcBrushDown, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mousemove', funcBrushMove, false);};}()),
        (function(){return function(){this.parent.layer.removeEventListener('mouseup', funcBrushUp, false);};}())
    );

    this.updateSvg.bind(this)();
};
// 軸上の SVG 要素の外観や大きさを、その時点での this が持つプロパティに応じてアップデートする
Axis.prototype.updateSvg = function(){
    var fill, stroke, display;
    var top, bottom;
    // 軸上の選択可能な領域
    if(this.brushed || this.onBrush){
        stroke = this.parent.AXIS_LINE_SELECT_COLOR;
    }else{
        stroke = 'transparent';
    }
    this.axisRectSvg.setAttribute('stroke', stroke);
    this.axisRectSvg.setAttribute(
        'd',
        'M ' + (this.centerH - this.parent.AXIS_SCALE_WIDTH) + ' ' + this.parent.SVG_TEXT_BASELINE +
        ' v ' + (this.height - this.parent.SVG_TEXT_BASELINE) +
        ' h ' + (this.parent.AXIS_SCALE_WIDTH * 2) +
        ' V ' + this.parent.SVG_TEXT_BASELINE +
        ' h ' + (-this.parent.AXIS_SCALE_WIDTH * 2)
    );
    // 軸上の選択された部分を表す矩形
    if(this.brushed || this.onBrush){
        display = '';
    }else{
        display = 'display: none;';
    }
    top = this.brushStartHeight * (this.height - this.parent.SVG_TEXT_BASELINE) + this.parent.SVG_TEXT_BASELINE;
    bottom = this.brushEndHeight * (this.height - this.parent.SVG_TEXT_BASELINE) + this.parent.SVG_TEXT_BASELINE;
    this.brushRectSvg.setAttribute(
        'd',
        'M ' + (this.centerH - this.parent.AXIS_SCALE_WIDTH) + ' ' + top +
        ' V ' + bottom +
        ' h ' + (this.parent.AXIS_SCALE_WIDTH * 2) +
        ' V ' + top +
        ' h ' + (-this.parent.AXIS_SCALE_WIDTH * 2)
    );
    this.brushRectSvg.setAttribute('style', display);
    // 軸上の選択領域の上下の先端部分（不可視だがBrush領域を拡縮するのに使う）※上端
    if((this.brushed && this.onBrush) || (this.brushed && this.onBrushRect)){
        display = '';
    }else if(this.brushed){
        display = 'cursor: row-resize;';
    }else{
        display = 'cursor: row-resize; display: none;';
    }
    this.brushTopRectSvg.setAttribute(
        'd',
        'M ' + (this.centerH - this.parent.AXIS_SCALE_WIDTH) + ' ' + (top - this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' V ' + (top + this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' h ' + (this.parent.AXIS_SCALE_WIDTH * 2) +
        ' V ' + (top - this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' h ' + (-this.parent.AXIS_SCALE_WIDTH * 2)
    );
    this.brushTopRectSvg.setAttribute('style', display);
    // 軸上の選択領域の上下の先端部分（不可視だがBrush領域を拡縮するのに使う）※下端
    this.brushBottomRectSvg.setAttribute(
        'd',
        'M ' + (this.centerH - this.parent.AXIS_SCALE_WIDTH) + ' ' + (bottom - this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' V ' + (bottom + this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' h ' + (this.parent.AXIS_SCALE_WIDTH * 2) +
        ' V ' + (bottom - this.parent.AXIS_BRUSHED_EDGE_HEIGHT) +
        ' h ' + (-this.parent.AXIS_SCALE_WIDTH * 2)
    );
    this.brushBottomRectSvg.setAttribute('style', display);
};
// 軸を削除するため listener を remove して HTML を空にする
Axis.prototype.delete = function(){
    var i, j;
    if(this.listeners.length > 0){
        for(i = 0, j = this.listeners.length; i < j; ++i){
            this.listeners[i].bind(this)();
        }
        this.listeners = [];
    }
    this.svg.innerHTML = '';
    this.parent.layer.removeChild(this.svg);
    this.svg = null;
};
// 軸を描画している実行部分
// 現状は軸を minmax の差分を用いて 10 分割している
// つまり、最小値も最大値も、きれいに軸の両端にぴったりと配置される
Axis.prototype.drawScale = function(){
    var i, j, k, l;
    var text, path, bbox, dummy;
    var smin, smax;
    var range = this.max - this.min;
    var scale = range / 10;
    smin = this.min;
    smax = this.max;
    l = this.svg.clientHeight - this.parent.SVG_TEXT_BASELINE;
    dummy = this.parent.NS('text');
    dummy.style.position = 'relative';
    dummy.style.fontSize = this.parent.SVG_SCALE_SIZE;
    dummy.style.visibility = 'hidden';
    this.svg.appendChild(dummy);
    for(i = this.min; i <= smax; i += scale){
        text = this.parent.NS('text');
        text.style.position = 'relative';
        text.style.overflow = 'visible';
        text.style.fontSize = this.parent.SVG_SCALE_SIZE;
        text.textContent = '' + this.formatFloat(i, 5);
        dummy.textContent = '' + this.formatFloat(i, 5);
        bbox = dummy.getBBox();
        j = bbox.width - (this.parent.SVG_DEFAULT_WIDTH / 2) + this.parent.AXIS_SCALE_WIDTH + 2;
        k = this.svg.clientHeight - ((i - this.min) / (smax - this.min)) * l;
        text.style.transform = 'translate(' + -j + 'px, ' + (k + 5) + 'px)';
        this.svg.appendChild(text);
        path = this.parent.NS('path');
        path.setAttribute('stroke', this.parent.AXIS_LINE_COLOR);
        path.setAttribute('stroke-width', this.parent.AXIS_LINE_WIDTH);
        path.setAttribute(
            'd',
            'M ' + this.centerH + ' ' + k + ' h ' + -this.parent.AXIS_SCALE_WIDTH
        );
        this.svg.appendChild(path);
    }
};
// 軸のスタイルを変更して直接横位置を設定する（非推奨）
Axis.prototype.setPosition = function(x){
    this.svg.style.left = x;
};
// 自身の minmax プロパティを直接設定する（非推奨）
Axis.prototype.setMinMax = function(min, max){
    this.min = min;
    this.max = max;
};
// 自身に格納しているクラスタの内容から minmax を求めて自身に設定する
Axis.prototype.getClustersMinMax = function(){
    if(this.clusters.length === 0){return;}
    var i, j, k, l;
    k = l = 0;
    if(this.clusters.length === 1){
        k = this.clusters[0].min;
        l = this.clusters[0].max;
    }else{
        for(i = 0, j = this.clusters.length; i < j; ++i){
            k = Math.min(this.clusters[i].min, k);
            l = Math.max(this.clusters[i].max, l);
        }
    }
    this.min = k;
    this.max = l;
    return this;
};
// 正規化していない軸の Left（ピクセル単位、0 始点）
Axis.prototype.getHorizontalRange = function(){
    // horizon range
    var i = parseFloat(this.svg.style.left.replace(/px$/));
    return i + (this.parent.SVG_DEFAULT_WIDTH / 2) + (this.index * this.parent.SVG_DEFAULT_WIDTH) - this.parent.PARALLEL_PADDING;
};
// 正規化した軸の横位置（0 ~ 1）
Axis.prototype.getNomalizeHorizontalRange = function(){
    // horizon normalize range
    var i = this.getHorizontalRange();
    return i / this.parent.drawRect.width;
};
// 軸タイトルのドラッグ開始イベント
Axis.prototype.dragStart = function(eve){
    this.left = eve.pageX;
    this.onAxisTitleDrag = true;
};
// 軸タイトルのドラッグイベント
Axis.prototype.dragMove = function(eve){
    if(!this.onAxisTitleDrag){return;}
    var x = eve.pageX - this.left;
    var df = parseFloat(this.svg.style.left.replace(/px$/, ''));
    var i = df + x;
    var j = this.parent.drawRect.width - ((this.index + 1) * this.parent.SVG_DEFAULT_WIDTH) + (this.parent.SVG_DEFAULT_WIDTH / 2) + this.parent.PARALLEL_PADDING;
    var k = this.parent.PARALLEL_PADDING - (this.index * this.parent.SVG_DEFAULT_WIDTH) - (this.parent.SVG_DEFAULT_WIDTH / 2);
    if(i > j){i = j;}
    if(i < k){i = k;}
    this.svg.style.left = i + 'px';
    this.left = eve.pageX;
    if(this.parent.glReady){
        this.parent.drawCanvas();
    }
};
// 軸タイトルのドラッグ終了イベント
Axis.prototype.dragEnd = function(eve){
    this.onAxisTitleDrag = false;
    // 時間差で軸リセットを呼ぶ
    setTimeout(this.parent.resetAxis.bind(this.parent), 300);
};
// 軸上でのドラッグ開始（Brush開始）イベント
Axis.prototype.dragAxisStart = function(eve){
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    this.brushed = true;
    this.onBrush = true;
    this.eventCurrentSvg = this.axisRectSvg;
    this.brushStartHeight = Math.max(0, Math.min(1.0, (eve.offsetY - this.parent.SVG_TEXT_BASELINE) / h));
    this.brushEndHeight = this.brushDefaultHeight = this.brushStartHeight;
};
// 軸上でのドラッグ（Brush中）イベント
Axis.prototype.dragAxisMove = function(eve){
    if(!this.onBrush){return;}
    if(this.eventCurrentSvg !== this.axisRectSvg){return;}
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    var t = Math.max(0, Math.min(1.0, (eve.offsetY - this.parent.SVG_TEXT_BASELINE) / h));
    if(t < this.brushDefaultHeight){
        this.brushStartHeight = t;
        this.brushEndHeight = this.brushDefaultHeight;
    }else if(t > this.brushDefaultHeight){
        this.brushEndHeight = t;
        this.brushStartHeight = this.brushDefaultHeight;
    }
    this.updateSvg.bind(this)();
};
// 軸上でのドラッグ終了（Brush完了）イベント
Axis.prototype.dragAxisEnd = function(eve){
    if(this.eventCurrentSvg !== this.axisRectSvg){return;}
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    this.onBrush = false;
    var t = Math.max(0, Math.min(1.0, (eve.offsetY - this.parent.SVG_TEXT_BASELINE) / h));
    if(t < this.brushDefaultHeight){
        this.brushStartHeight = t;
        this.brushEndHeight = this.brushDefaultHeight;
    }else if(t > this.brushDefaultHeight){
        this.brushEndHeight = t;
        this.brushStartHeight = this.brushDefaultHeight;
    }
    if(this.brushEndHeight - this.brushStartHeight <= 0.03){
        this.brushed = false;
        this.brushStartHeight = this.brushEndHeight = 0.0;
    }
    this.updateSvg.bind(this)();

    var axisjson = this.parent.getAllBrushedRange(this);
    if(this.parent.selectedCallback){this.parent.selectedCallback('brushjson', axisjson);}
};
// 軸の上下のハンドルをドラッグ開始
Axis.prototype.dragAxisHandleStart = function(eve){
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    var y = eve.currentTarget.getBBox().y + this.parent.AXIS_BRUSHED_EDGE_HEIGHT;
    this.brushed = true;
    this.onBrush = true;
    this.eventCurrentSvg = this.axisRectSvg;
    var v = this.getBrushedRange();
    if(eve.currentTarget === this.brushTopRectSvg){
        this.brushDefaultHeight = v.bottom;
        this.brushStartHeight = v.top;
        this.brushEndHeight = v.bottom;
    }else{
        this.brushDefaultHeight = v.top;
        this.brushStartHeight = v.top;
        this.brushEndHeight = v.bottom;
    }
};
// 軸上の選択済みエリアをドラッグ開始
Axis.prototype.dragAxisBrushStart = function(eve){
    this.onBrushRect = true;
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    var n = (eve.offsetY - this.parent.SVG_TEXT_BASELINE) / h;
    this.brushRectDefaultHeight = n;
    this.eventCurrentSvg = this.brushRectSvg;
};
// 軸上の選択済みエリアをドラッグ中
Axis.prototype.dragAxisBrushMove = function(eve){
    if(this.eventCurrentSvg !== this.brushRectSvg){return;}
    if(!this.onBrushRect){return;}
    var v = this.getBrushedRange();
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    var n = (eve.offsetY - this.parent.SVG_TEXT_BASELINE) / h;
    var offset = n - this.brushRectDefaultHeight;
    if(offset + v.top < 0.0){
        offset += -(offset + v.top);
    }
    if(offset + v.bottom > 1.0){
        offset += 1.0 - (offset + v.bottom);
    }
    this.brushStartHeight += offset;
    this.brushEndHeight += offset;
    this.brushRectDefaultHeight += offset;
    this.updateSvg.bind(this)();
};
// 軸上の選択済みエリアをドラッグ終了
Axis.prototype.dragAxisBrushEnd = function(eve){
    if(this.eventCurrentSvg !== this.brushRectSvg){return;}
    if(!this.onBrushRect){return;}
    this.onBrushRect = false;

    var axisjson = this.parent.getAllBrushedRange(this);
    if(this.parent.selectedCallback){this.parent.selectedCallback('brushjson', axisjson);}
};
// 軸上の選択範囲を正規化した値として返す
Axis.prototype.getBrushedRange = function(){
    if(!this.brushed){return null;}
    var h = this.height - this.parent.SVG_TEXT_BASELINE;
    var t = this.parent.AXIS_BRUSHED_EDGE_HEIGHT - this.parent.SVG_TEXT_BASELINE;
    var top    = Math.max(0, Math.min(1.0, (this.brushTopRectSvg.getBBox().y    + t) / h));
    var bottom = Math.max(0, Math.min(1.0, (this.brushBottomRectSvg.getBBox().y + t) / h));
    return {
        top: top,
        bottom: bottom,
        index: this.index,
        length: bottom - top
    };
};
// クラスタの選択状態をbrushを元に判定して配列で返す
Axis.prototype.getClusterBrushed = function(){
    var i, j, v, a = [];
    var min, max, len;
    if(!this.brushed){return new Array(this.clusters.length);}
    v = this.getBrushedRange();
    for(i = 0, j = this.clusters.length; i < j; ++i){
        len = this.max - this.min;
        min = len * (1.0 - v.bottom) + this.min;
        max = len * (1.0 - v.top)    + this.min;
        a.push((
            (this.clusters[i].min <= min && this.clusters[i].max >= min) ||
            (this.clusters[i].min <= max && this.clusters[i].max >= max) ||
            (this.clusters[i].min >= min && this.clusters[i].max <= max)
        ));
    }
    return a;
};

Axis.prototype.formatFloat = function(number, n){
    var p = Math.pow(10, n);
    return Math.round(number * p) / p;
};

// cluster ================================================================
function Cluster(axis, index, selected, out, min, max, top, color){
    var c;
    this.parentAxis = axis;   // 自分自身が所属する軸インスタンス
    this.index = index;       // 自分自身のインデックス
    this.selected = selected; // 
    this.out = out;           // 
    this.min = min;           // 自分自身の最小値
    this.max = max;           // 自分自身の最大値
    this.top = top;           // 自分自身の突出頂点部分の値
    this.color = color || [0, 0, 0, 1]; // 色
    if(!this.parentAxis.putData.right){
        c = this.getInputPower();
    }else{
        c = this.getOutputPower();
    }
    this.color[3] *= c;
    this.color[3] = this.color[3] * 0.9 + 0.1;
    return this;
}
// 正規化された、クラスタの縦方向の位置の上辺と下辺
Cluster.prototype.getNormalizeRange = function(){
    // vertical normalize range
    var i = this.parentAxis.max - this.parentAxis.min;
    var t = (this.top - this.min) / (this.max - this.min);
    return {
        min: (this.min - this.parentAxis.min) / i,
        max: 1.0 - (this.parentAxis.max - this.max) / i,
        top: t,
        percentage: (this.max - this.min) / (this.parentAxis.max - this.parentAxis.min)
    };
};
// クラスタ自身にインプットされている量/軸
Cluster.prototype.getInputPower = function(){
    var i, j;
    var data = this.parentAxis.putData.left;
    if(!data){return 0;}
    j = 0;
    for(i = 0; i < data.length; ++i){
        j += data[i][this.index];
    }
    return j / this.parentAxis.dataLength;
};
// クラスタ自身がアウトプットしている量/軸
Cluster.prototype.getOutputPower = function(){
    var i, j;
    var data = this.parentAxis.putData.right;
    if(!data){return 0;}
    j = 0;
    for(i = 0; i < data[this.index].length; ++i){
        j += data[this.index][i];
    }
    return j / this.parentAxis.dataLength;
};
// クラスタ自身がアウトプットしているラインの各量の比率
Cluster.prototype.getOutputLinePower = function(){
    var i, j, a = [];
    var data = this.parentAxis.putData.right;
    if(!data){return 0;}
    j = 0;
    for(i = 0; i < data[this.index].length; ++i){
        a.push(data[this.index][i] / this.parentAxis.dataLength);
    }
    return a;
};
Cluster.prototype.setSelected = function(select){
    this.selected = select;
};
Cluster.prototype.setColor = function(color){
    this.color = color;
};

// util ===================================================================
// json ロードするやーつ
function loadJSON(url, callback){
    var xml = new XMLHttpRequest();
    xml.open('GET', url, true);
    xml.onload = function(){
        callback(xml.responseText);
    };
    xml.send();
}

