import React from 'react';
import ReactDOM from "react-dom";

class TransferFunction extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action  = props.action;
        this.node = props.node;

        // members
        this.wrapper = null;
        this.cw = 300;
        this.colorbarHeight = 20;
        this.canvas = null;
        this.ctx = null;
        this.colorctx  = null;
        this.redbtn    = null;
        this.greenbtn  = null;
        this.bluebtn   = null;
        this.alphabtn  = null;
        this.minInput  = null;
        this.maxInput  = null;
        this.graphMode = null;
        this.changeCallback = null;
        this.defaultValMin = 0;
        this.defaultValMax = 0;
        const numVals = 256;
        this.numVals    = numVals;
        this.valueRed   = [numVals];
        this.valueGreen = [numVals];
        this.valueBlue  = [numVals];
        this.valueAlpha = [numVals];
        this.hist = [numVals];
        for(let i = 0; i < numVals; ++i){
            this.valueRed[i] = i/numVals;
            this.valueGreen[i] = 0.8;
            this.valueBlue[i] = 1.0 - i / numVals;
            this.valueAlpha[i] = 1.0;
            this.hist[i] = 0;
        }
        this.oldx = 0;
        this.oldy = 0;
        this.disableColor = '#8b8b8b';
        this.mspress = false;

        // state
        this.state = {
            selValue: "0",
            selValues: [
                {value: "0", text: "Custom"},
                {value: "1", text: "Blue and Red"},
                {value: "2", text: "Black and White"},
                {value: "3", text: "BGR Gradation"}
            ],
            valMin: this.props.node.input[2].value,
            valMax: this.props.node.input[3].value,
            btnFlags: [],
            redbtnColor: this.disableColor,
            greenbtnColor: this.disableColor,
            bluebtnColor: this.disableColor,
            alphabtnColor: this.disableColor,
            allbtnColor: '#fff'
        };

        // methods
        this.onRecieveAnalyzed   = this.onRecieveAnalyzed.bind(this);
        this.onSelectChange      = this.onSelectChange.bind(this);
        this.redbtnClick         = this.redbtnClick.bind(this);
        this.greenbtnClick       = this.greenbtnClick.bind(this);
        this.bluebtnClick        = this.bluebtnClick.bind(this);
        this.alphabtnClick       = this.alphabtnClick.bind(this);
        this.allbtnClick         = this.allbtnClick.bind(this);
        this.allBtnOff           = this.allBtnOff.bind(this);
        this.minInputChange      = this.minInputChange.bind(this);
        this.maxInputChange      = this.maxInputChange.bind(this);
        this.componentDidMount   = this.componentDidMount.bind(this);
        this.componentWillUnmount= this.componentWillUnmount.bind(this);
        this.transFunc           = this.transFunc.bind(this);
        this.invTransFunc        = this.invTransFunc.bind(this);
        this.init                = this.init.bind(this);
        this.drawGraph           = this.drawGraph.bind(this);
        this.drawValueLine       = this.drawValueLine.bind(this);
        this.drawColorBar        = this.drawColorBar.bind(this);
        this.mouseMoveFunc       = this.mouseMoveFunc.bind(this);
        this.setGraphValue       = this.setGraphValue.bind(this);
        this.setMaskedGraphValue = this.setMaskedGraphValue.bind(this);
        this.getGraphValueRed    = this.getGraphValueRed.bind(this);
        this.getGraphValueGreen  = this.getGraphValueGreen.bind(this);
        this.getGraphValueBlue   = this.getGraphValueBlue.bind(this);
        this.getGraphValueAlpha  = this.getGraphValueAlpha.bind(this);
        this.getMinValue         = this.getMinValue.bind(this);
        this.getMaxValue         = this.getMaxValue.bind(this);
        this.getNumValues        = this.getNumValues.bind(this);
        this.setAnalyzeResult    = this.setAnalyzeResult.bind(this);
        this.styles              = this.styles.bind(this);

        this.changeCallback = () => {
            let rgba = [4*numVals];
            for(let i = 0; i < numVals; ++i){
                rgba[4*i  ] = parseInt(this.valueRed[i]*255);
                rgba[4*i+1] = parseInt(this.valueGreen[i]*255);
                rgba[4*i+2] = parseInt(this.valueBlue[i]*255);
                rgba[4*i+3] = parseInt(this.valueAlpha[i]*255);
            }
            //this.node.rgba
            const varname = this.node.varname;
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "rgba" : rgba
                }
            });
        };
     }
     onRecieveAnalyzed(data) {
        //console.error(data);
        this.setAnalyzeResult(data);
     }
     onSelectChange(eve){
        var e = eve.target.value;
        var t = parseInt(e);
        this.setState({selValue: t});
        if (t === 1) {
            for (let i = 0; i < this.numVals; ++i){
                this.valueRed[i]   = i / this.numVals;
                this.valueGreen[i] = 0.0;
                this.valueBlue[i]  = 1.0 - i / this.numVals;
                this.valueAlpha[i] = 1.0;
            }
        } else if (t === 2) {
            for (let i = 0; i < this.numVals; ++i){
                this.valueRed[i]   = i / this.numVals;
                this.valueGreen[i] = i / this.numVals;
                this.valueBlue[i]  = i / this.numVals;
                this.valueAlpha[i] = 1.0;
            }
        } else if (t === 3) {
            for (let i = 0; i < this.numVals; ++i){
                this.valueRed[i]   = i / this.numVals;
                this.valueGreen[i] = Math.sin(i / this.numVals * Math.PI);
                this.valueBlue[i]  = 1.0 - i / this.numVals;
                this.valueAlpha[i] = 1.0;
            }
        }

        this.drawGraph();
        if (this.changeCallback){
            this.changeCallback(this);
        }
    }
    redbtnClick(){
        this.graphMode = 1;
        this.allBtnOff();
        this.setState({redbtnColor: '#f00'});
    }
    greenbtnClick(){
        this.graphMode = 2;
        this.allBtnOff();
        this.setState({greenbtnColor: '#0f0'});
    }
    bluebtnClick(){
        this.graphMode = 4;
        this.allBtnOff();
        this.setState({bluebtnColor: '#11f'});
    }
    alphabtnClick(){
        this.graphMode = 8;
        this.allBtnOff();
        this.setState({alphabtnColor: '#000'});
    }
    allbtnClick(){
        this.graphMode = (1|2|4);
        this.allBtnOff();
        this.setState({allbtnColor: '#fff'});
    }
    allBtnOff(){
        this.setState({
            redbtnColor:   this.disableColor,
            bluebtnColor:  this.disableColor,
            greenbtnColor: this.disableColor,
            alphabtnColor: this.disableColor,
            allbtnColor:   this.disableColor
        });
    }
    minInputChange(eve){
        if (parseFloat(this.minInput.value) !== NaN) {
            const vl = this.minInput.value;
            // const vl = parseFloat(this.minInput.value);
            const varname = this.node.varname;
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "minval" : vl
                }
            });
            this.setState({valMin: vl});
            this.drawGraph();
            if (this.changeCallback){
                this.changeCallback(this);
            }
        }
    }
    maxInputChange(eve){
        if (parseFloat(this.maxInput.value) !== NaN) {
            const vl = parseFloat(this.maxInput.value);
            const varname = this.node.varname;
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "maxval" : vl
                }
            });
            this.setState({valMax: vl});
            this.drawGraph();
            if (this.changeCallback){
                this.changeCallback(this);
            }
        }
    }
    componentDidMount(){
        this.wrapper = ReactDOM.findDOMNode(this.refs.wrapper);
        this.graphMode = (1|2|4);
        this.init();
        this.drawGraph();
        this.canvas.addEventListener('mousedown', ((eve)=>{
            this.oldx = eve.clientX - this.wrapper.getBoundingClientRect().left;
            this.oldy = eve.clientY - this.wrapper.getBoundingClientRect().top;
            this.mspress = true;
            document.addEventListener('mousemove', this.mouseMoveFunc, true);
        }).bind(this), true);
        document.addEventListener('mouseup', (()=>{
            this.mspress = false;
            document.removeEventListener('mousemove', this.mouseMoveFunc);
        }).bind(this), true);

        const ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        this.store.on(ANALYZED_DATA_RECIEVED, this.onRecieveAnalyzed);
    }
    componentWillUnmount() {
        const ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        this.store.off(ANALYZED_DATA_RECIEVED, this.onRecieveAnalyzed);
    }
    transFunc(x){return Math.sqrt(x);}
    invTransFunc(x){return x*x;}
    init(){
        this.canvas       = ReactDOM.findDOMNode(this.refs.canvas);
        var preSelect     = ReactDOM.findDOMNode(this.refs.preSelect);
        var colorframe    = ReactDOM.findDOMNode(this.refs.colorframe);
        colorframe.width  = this.cw;
        colorframe.height = this.colorbarHeight;
        this.colorctx     = colorframe.getContext ('2d');
        this.redbtn       = ReactDOM.findDOMNode(this.refs.redbtn);
        this.greenbtn     = ReactDOM.findDOMNode(this.refs.redbtn);
        this.bluebtn      = ReactDOM.findDOMNode(this.refs.redbtn);
        this.alphabtn     = ReactDOM.findDOMNode(this.refs.redbtn);
        this.allbtn       = ReactDOM.findDOMNode(this.refs.redbtn);
        this.minInput     = ReactDOM.findDOMNode(this.refs.minInput);
        this.maxInput     = ReactDOM.findDOMNode(this.refs.maxInput);

        this.canvas.width  = this.cw;
        this.canvas.height = this.cw;
        this.ctx = this.canvas.getContext ('2d');
    }
    drawGraph(){
        var cw = this.cw;
        var ch = this.cw;

        // clear
        this.ctx.beginPath();
        this.ctx.clearRect(0,0,cw,ch);

        // waku
        this.ctx.beginPath();
        this.ctx.fillStyle = "rgb(60, 60, 60)";
        this.ctx.fillRect(0,0,cw,ch);
        this.ctx.strokeStyle = "#676767";
        for (var x = 0; x <= cw; x += 50){
            this.ctx.moveTo(x,0);
            this.ctx.lineTo(x,ch);
        }
        for (var y = 0; y <= ch; y += 50){
            var yv = this.transFunc(y/ch);
            this.ctx.moveTo(0 ,(1.0 - yv)*ch);
            this.ctx.lineTo(cw,(1.0 - yv)*ch);
        }
        this.ctx.font = "16px";
        this.ctx.fillStyle = "#8e8e8e";
        this.ctx.fillText("1.0", 5, 20);
        this.ctx.fillText("0.0", 5, ch-30);
        this.ctx.fillText("min", 20, ch-5);
        this.ctx.fillText("max", cw - 40, ch-5);
        this.ctx.stroke();

        this.ctx.globalCompositeOperation = 'lighter';
        /*this.drawValueLine(this.values, '#AAA');
          this.drawValueLine(this.histRed, '#F00');
          this.drawValueLine(this.histGreen, '#0F0');
          this.drawValueLine(this.histBlue, '#00F');*/
        this.drawValueLine(this.valueRed, '#F00');
        this.drawValueLine(this.valueGreen, '#0F0');
        this.drawValueLine(this.valueBlue, '#11F');

        this.drawColorBar(this.valueRed, this.valueGreen, this.valueBlue, this.valueAlpha);
        this.ctx.globalCompositeOperation = 'source-over';
        this.drawValueLine(this.valueAlpha, '#000');
        this.drawValueLine(this.hist, 'rgb(154, 79, 40)', true);
    }
    drawValueLine(vals, col, scaling){
        var cw = this.cw;
        var ch = this.cw;
        // Graph
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        this.ctx.strokeStyle = col;
        this.ctx.moveTo(0,ch);

        var defDiff = this.defaultValMax - this.defaultValMin,
            valDiff = this.state.valMax - this.state.valMin,
            gscale = 1.0,
            goffset = 0.0;

        if (scaling) {
            goffset = (this.defaultValMin - this.state.valMin) / defDiff;
            gscale = defDiff / valDiff;
        }
        for (var i = 0; i < this.numVals; ++i){
            //this.ctx.lineTo(i / this.numVals *  cw, (1.0 - vals[i]) * ch); // MODE NORMAL
            this.ctx.lineTo(((i / this.numVals + goffset)  * gscale) *  cw, ch - this.transFunc(vals[i]) * ch); // MODE SQRT
        }
        this.ctx.stroke();
        this.ctx.lineWidth = 1;
    }
    drawColorBar(red, green, blue, alpha) {
        var colorData = this.colorctx.getImageData(0,0, this.cw, this.colorbarHeight),
            i, j, iw  = this.cw * 4;

        for (j = 0; j < this.colorbarHeight; j++) {
            for (i = 0; i < this.cw; i++) {
                var d = i * 4,
                    B,
                    s = Math.floor(i * this.numVals / this.cw),
                        backgrd = (1.0 - alpha[s]) * ((((i/10)|0)%2) ^ (((j/10)|0)%2)) * 155;
                        colorData.data[j * iw + d + 0] = ((red[s]  * 255 * alpha[s] + backgrd)| 0);
                        colorData.data[j * iw + d + 1] = ((green[s]* 255 * alpha[s] + backgrd)| 0);
                        colorData.data[j * iw + d + 2] = ((blue[s] * 255 * alpha[s] + backgrd)| 0);
                        colorData.data[j * iw + d + 3] = 255;
            }
        }
        this.colorctx.putImageData(colorData, 0, 0);
    }
    mouseMoveFunc(eve){
        var cw = this.cw;
        var ch = this.cw;
        var x = eve.clientX - this.wrapper.getBoundingClientRect().left,
            y = eve.clientY - this.wrapper.getBoundingClientRect().top;
            x = Math.min(x, this.cw);
            x = Math.min(cw, Math.max(0.0, x));
            y = Math.min(ch, Math.max(0.0, y));
        if (this.mspress){
            var minx = Math.min(x, this.oldx),
                maxx = Math.max(x, this.oldx),
                miny = this.oldy,//Math.min(y, this.oldy),
                maxy = y,//Math.max(y, this.oldy),
                i;
            if (x < this.oldx) {
                i = miny;
                miny = maxy;
                maxy = i;
            }

            for (i = minx+1; i <= maxx; ++i) {
                var dy = (maxy - miny) * (i - minx+1)/(maxx - minx+1) + miny;
                //this.setMaskedGraphValue(Math.floor(i/cw*this.numVals), 1.0 - dy/ch); // MODE NORMAL
                this.setMaskedGraphValue(Math.floor(i/cw*this.numVals), this.invTransFunc((ch-dy)/ch)); // MODE SQRT
            }

            this.drawGraph();
            if (this.changeCallback){
                this.changeCallback(this);
            }
        }
        this.oldx = x;
        this.oldy = y;
    }
    setGraphValue(n, val_r, val_g, val_b, val_a){
        if(n < 0 || this.numVals <= n){return;}
        this.valueRed[n]   = val_r;
        this.valueGreen[n] = val_g;
        this.valueBlue[n]  = val_b;
        this.valueAlpha[n] = val_a;
    }
    setMaskedGraphValue(n, val){
        if (n < 0 || this.numVals <= n){return;}
        val = Math.max(0.0, Math.min(1.0, val));
        if (this.graphMode & 1) {
            this.valueRed[n] = val;
        }
        if (this.graphMode & 2) {
            this.valueGreen[n] = val;
        }
        if (this.graphMode & 4) {
            this.valueBlue[n] = val;
        }
        if (this.graphMode & 8) {
            this.valueAlpha[n] = val;
        }
    }
    getGraphValueRed(){
        return this.valueRed;
    }
    getGraphValueGreen(){
        return this.valueGreen;
    }
    getGraphValueBlue(){
        return this.valueBlue;
    }
    getGraphValueAlpha(){
        return this.valueAlpha;
    }
    getMinValue(){
        return this.state.valMin;
    }
    getMaxValue(){
        return this.state.valMax;
    }
    setMinValue(val){
        this.setState({valMin: val});
    }
    setMaxValue(val){
        this.setState({valMax: val});
    }
    getNumValues(){
        return this.numVals;
    }
    setAnalyzeResult(result) {
        if (result) {
            if (result.histgram) {
                const hist = result.histgram;
                for (let i = 0; i < this.numVals; ++i) {
                    this.hist[i] = hist[i];
                }
            }
            let min = parseFloat(result.minval);
            let max = parseFloat(result.maxval);
            const varname = this.node.varname;
            if(this.defaultValMin !== min || this.defaultValMax !== max){
                this.defaultValMin = min;
                this.defaultValMax = max;
                this.action.changeNodeInput({
                    varname : varname,
                    input : {
                        minval :min,
                        maxval :max
                    }
                });
                this.setState({
                    valMin: min,
                    valMax: max
                });
                this.drawGraph();
            }
        }
    }
    /*setAnalyzeResult(result, component){
        var i, hist, componentNum;
        if (result && result.histgram) {
            componentNum = (result.type === "vector" ? 3 : 1);
            if (result.histgram) {
                hist = result.histgram;
                for (i = 0; i < this.numVals; ++i) {
                    this.hist[i] = hist[i * componentNum + component];
                }
            }

            this.defaultValMin = result.defaultValMin;
            this.defaultValMax = result.defaultValMax;
            this.setState({
                valMin: result.min[component],
                valMax: result.max[component]
            });
        }
    }*/

    styles(){
        return {
            wrapper: {
                width: "300px",
            },
            canvas: {},
            colorframe: {
                lineHeight: "30px",
                width: "300px",
                height: "20px"
            },
            btnframe: {
                textAlign: "center",
                width: "300px",
                height: "30px",
                display: "flex",
                flexDirection: "row"
            },
            redbtn:   {backgroundColor: this.state.redbtnColor  , margin: "auto 1px",  width: "58px", height: "20px", fontSize: "13px", color: "#555", borderRadius: "3px", cursor: "pointer"},
            greenbtn: {backgroundColor: this.state.greenbtnColor, margin: "auto 1px",  width: "58px", height: "20px", fontSize: "13px", color: "#555", borderRadius: "3px", cursor: "pointer"},
            bluebtn:  {backgroundColor: this.state.bluebtnColor , margin: "auto 1px",  width: "58px", height: "20px", fontSize: "13px", color: "#555", borderRadius: "3px", cursor: "pointer"},
            alphabtn: {backgroundColor: this.state.alphabtnColor, margin: "auto 1px",  width: "58px", height: "20px", fontSize: "13px", color: "#555", borderRadius: "3px", cursor: "pointer"},
            allbtn:   {backgroundColor: this.state.allbtnColor  , margin: "auto 1px",  width: "58px", height: "20px", fontSize: "13px", color: "#555", borderRadius: "3px", cursor: "pointer"},
            presetframe: {
                lineHeight: "30px",
                height: "30px",
                display: "flex",
                flexDirection: "row"
            },
            preText: {
                padding: "2px",
                width: "80px",
                color: "#fff",
            },
            preSelect: {
                margin: "auto 0px",
                width: "200px"
            },
            minmaxframe: {
                height: "30px",
                display: "flex",
                flexDirection: "row"
            },
            numberArea: {
                flex: "1 0 auto"
            },
            minmaxText: {
                color: "#fff",
                fontSize: "small",
                padding: "2px",
                width: "40px",
                display: "inline-block"
            },
            minmaxInput: {
                border: "none",
                borderRadius: "3px 5px",
                padding: "3px",
                width: "90px",
            },
        };
    }

    render(){
        const styles = this.styles();
        const optionGenerator = this.state.selValues.map(((value, key)=>{
            return (
                <option value={value.value} key={Date.now() + ':' + key}>{value.text}</option>
            );
        }).bind(this));
        return (
            <div ref="wrapper" style={styles.wrapper}>
                <canvas ref="canvas" style={styles.canvas}></canvas>
                <canvas ref="colorframe" style={styles.colorframe}></canvas>
                <div ref="btnframe" style={styles.btnframe}>
                    <div ref="redbtn"   onClick={this.redbtnClick}   style={styles.redbtn}>RED</div>
                    <div ref="greenbtn" onClick={this.greenbtnClick} style={styles.greenbtn}>GREEN</div>
                    <div ref="bluebtn"  onClick={this.bluebtnClick}  style={styles.bluebtn}>BLUE</div>
                    <div ref="alphabtn" onClick={this.alphabtnClick} style={styles.alphabtn}>ALPHA</div>
                    <div ref="allbtn"   onClick={this.allbtnClick}   style={styles.allbtn}>RGB</div>
                </div>
                <div ref="presetframe" style={styles.presetframe}>
                    <div ref="preText" className="KCaption" style={styles.preText}>Preset</div>
                    <select ref="preSelect" className="KDropdownList"
                            value={this.state.selValue}
                            onChange={this.onSelectChange}
                            style={styles.preSelect}
                    >
                        {optionGenerator}
                    </select>
                </div>
                <div ref="minmaxframe" style={styles.minmaxframe}>
                    <div style={styles.numberArea}>
                        <div ref="minText" className="KCaption" style={styles.minmaxText}>Min</div>
                        <input ref="minInput" type="number" step={0.00001} value={this.state.valMin} onChange={this.minInputChange} style={styles.minmaxInput} />
                    </div>
                    <div style={styles.numberArea}>
                        <div ref="maxText" className="KCaption" style={styles.minmaxText}>Max</div>
                        <input ref="maxInput" type="number" step={0.00001} value={this.state.valMax} onChange={this.maxInputChange} style={styles.minmaxInput} />
                    </div>
                </div>
            </div>
        );
    }
}

module.exports = TransferFunction;

// =============================================================

// KTransferFunctionClass.Init = function() {
//     var lst = document.getElementsByClassName('KTransferFunction');
//     for (var i=0; i<lst.length;++i) {
//         _kvtools_UIs[lst[i].id] = new KTransferFunctionClass(lst[i]);
//     }
// };
//
// KTransferFunctionClass.userInit = function(dom) {
//     var el = dom.getElementsByClassName('KTransferFunction');
//     for (var k = 0; k < el.length; ++k) {
//         _kvtools_UIs[el[k].id] = new KTransferFunctionClass(el[k]);
//     }
// };
//

