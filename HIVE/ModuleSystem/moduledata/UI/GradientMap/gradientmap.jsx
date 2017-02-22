import React from 'react';
import ReactDOM from "react-dom";

class GradientMap extends React.Component {
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
        this.minInput  = null;
        this.maxInput  = null;
        this.changeCallback = null;
        this.defaultValMin = this.props.node.input[4].value;
        this.defaultValMax = this.props.node.input[5].value;
        const numVals = 256;
        this.numVals    = numVals;
        this.value = [numVals];
        this.hist = [numVals];
        for(let i = 0; i < numVals; ++i){
            this.value[i] = 1.0;
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
                {value: "1", text: "RAMP"},
            ],
            valMin: null,//this.props.node.input[2].value,
            valMax: null,//this.props.node.input[3].value,
            btnFlags: [],
        };

        // methods
        this.onRecieveAnalyzed   = this.onRecieveAnalyzed.bind(this);
        this.onSelectChange      = this.onSelectChange.bind(this);
        this.minInputChange   = this.minInputChange.bind(this);
        this.maxInputChange   = this.maxInputChange.bind(this);
        this.nodeInputChanged    = this.nodeInputChanged.bind(this);
        this.componentDidMount   = this.componentDidMount.bind(this);
        this.componentWillUnmount= this.componentWillUnmount.bind(this);
        this.transFunc           = this.transFunc.bind(this);
        this.invTransFunc        = this.invTransFunc.bind(this);
        this.init                = this.init.bind(this);
        this.drawGraph           = this.drawGraph.bind(this);
        this.drawValueLine       = this.drawValueLine.bind(this);
        this.drawColorBar        = this.drawColorBar.bind(this);
        this.mouseMoveFunc       = this.mouseMoveFunc.bind(this);
        this.mouseDownFunc       = this.mouseDownFunc.bind(this);
        this.mouseUpFunc       = this.mouseUpFunc.bind(this);
        this.setMaskedGraphValue = this.setMaskedGraphValue.bind(this);
        this.getGraphValue  = this.getGraphValue.bind(this);
        this.getMinValue         = this.getMinValue.bind(this);
        this.getMaxValue         = this.getMaxValue.bind(this);
        this.getNumValues        = this.getNumValues.bind(this);
        this.setAnalyzeResult    = this.setAnalyzeResult.bind(this);
        this.undo                = this.undo.bind(this);
        this.redo                = this.redo.bind(this);
        this.styles              = this.styles.bind(this);
        this.undoBuffer = [JSON.stringify(this.value)];
        this.redoBuffer = [];

        this.changeCallback = () => {
            let rgba = [numVals * 4];
            for(let i = 0; i < numVals; ++i){
                rgba[i * 4 + 0] = parseInt(this.value[i]*255);
                rgba[i * 4 + 1] = rgba[i * 4 + 0]; 
                rgba[i * 4 + 2] = rgba[i * 4 + 0];
                rgba[i * 4 + 3] = 0xFF
            }
            //this.node.rgba
            const varname = this.node.varname;
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "values" : rgba
                }
            });
        };
     }
     onRecieveAnalyzed(data) {
        this.setAnalyzeResult(data);
     }
     onSelectChange(eve){
        var e = eve.target.value;
        var t = parseInt(e);
        this.setState({selValue: t});

        if (t === 1) {
            for (let i = 0; i < this.numVals; ++i){
                this.value[i] = i / this.numVals;
            }
        }

        this.drawGraph();
        if (this.changeCallback){
            this.changeCallback();
            this.redoBuffer = [];
        }
    }
    
	onMinKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submitMin.bind(this)(ev);
		}
	}

	onMinBlur(ev) {
		this.submitMin.bind(this)(ev);
		this.currentEdit = {
			value : null
		};
	}

	onMinFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}

	onMaxKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submitMax.bind(this)(ev);
		}
	}

	onMaxBlur(ev) {
		this.submitMax.bind(this)(ev);
		this.currentEdit = {
			value : null
		};
	}

	onMaxFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}
    
    submitMin(ev) {
        if(!isNaN(parseFloat(this.state.valMin))){
            const varname = this.node.varname;
            const minval = parseFloat(this.state.valMin);
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "minval": minval
                }
            });
        }
		ev.target.style.border = "none";
		ev.target.blur();
    }
    
    submitMax(ev) {
        if(!isNaN(parseFloat(this.state.valMax))){
            const varname = this.node.varname;
            const maxval = parseFloat(this.state.valMax);
            this.action.changeNodeInput({
                varname : varname,
                input : {
                    "maxval": maxval
                }
            });
        }
		ev.target.style.border = "none";
		ev.target.blur();
    }

    minInputChange(eve){
        this.setState({
            valMin : eve.target.value
        });
    }
    
    maxInputChange(eve){
        this.setState({
            valMax : eve.target.value
        });
    }
    
    nodeInputChanged(err, data){
        const varname = this.node.varname;
        if (varname !== data.varname){return;}

		if (data.input[0].value.length >= this.numVals) {
			for (let i = 0; i < this.numVals; ++i) {
				this.value[i] = data.input[0].value[i * 4 + 0] / 0xFF;
			}
		}

		this.setState({
			valMin: data.input[2].value,
			valMax: data.input[3].value
		});

		if (this.state.valMin !== data.input[2].value || this.state.valMax !== data.input[3].value) {
	        setTimeout((()=>{
                if (this.changeCallback) {
                    this.changeCallback();
                }
            }).bind(this), 50);
		}
        this.drawGraph();
    }
    componentDidMount(){
        this.wrapper = ReactDOM.findDOMNode(this.refs.wrapper);
        this.init();
        this.canvas.addEventListener('mousedown', this.mouseDownFunc);
        window.addEventListener('mouseup', this.mouseUpFunc);

        const ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.on(ANALYZED_DATA_RECIEVED, this.onRecieveAnalyzed);
        this.store.on(NODE_INPUT_CHANGED, this.nodeInputChanged);
    }
    componentWillUnmount() {
        this.canvas.removeEventListener('mousedown', this.mouseDownFunc);
        window.removeEventListener('mouseup', this.mouseUpFunc);
        
        const ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.off(ANALYZED_DATA_RECIEVED, this.onRecieveAnalyzed);
        this.store.off(NODE_INPUT_CHANGED, this.nodeInputChanged);
    }
    
    mouseDownFunc(eve) {
        this.oldx = eve.clientX - this.wrapper.getBoundingClientRect().left;
        this.oldy = eve.clientY - this.wrapper.getBoundingClientRect().top;
        this.mspress = true;
        document.addEventListener('mousemove', this.mouseMoveFunc);
    }
    
    mouseUpFunc(eve) {
        if (this.mspress) {
            if (this.undoBuffer.length > 30) {
                this.undoBuffer.pop();
            }
            this.undoBuffer.unshift(JSON.stringify(this.value));
        }
        this.mspress = false;
        document.removeEventListener('mousemove', this.mouseMoveFunc);
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
        this.minInput     = ReactDOM.findDOMNode(this.refs.minInput);
        this.maxInput     = ReactDOM.findDOMNode(this.refs.maxInput);

        this.canvas.width  = this.cw;
        this.canvas.height = this.cw;
        this.ctx = this.canvas.getContext ('2d');
		this.nodeInputChanged(null, this.props.node);
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

        this.drawColorBar(this.value, this.value, this.value, 1.0);
        this.ctx.globalCompositeOperation = 'source-over';
        this.drawValueLine(this.value, '#000');
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
                        colorData.data[j * iw + d + 0] = ((red[s]  * 255)| 0);
                        colorData.data[j * iw + d + 1] = ((green[s]* 255)| 0);
                        colorData.data[j * iw + d + 2] = ((blue[s] * 255)| 0);
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
                this.setMaskedGraphValue(Math.floor(i/cw*this.numVals), this.invTransFunc((ch-dy)/ch)); // MODE SQRT
            }

            this.drawGraph();
            if (this.changeCallback){
                this.changeCallback();
                this.redoBuffer = [];
            }
        }
        this.oldx = x;
        this.oldy = y;
    }
    setMaskedGraphValue(n, val){
        if (n < 0 || this.numVals <= n){return;}
        val = Math.max(0.0, Math.min(1.0, val));
        this.value[n] = val;
    }
    getGraphValue(){
        return this.value;
    }
    getMinValue(){
        return this.state.valMin;
    }
    getMaxValue(){
        return this.state.valMax;
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
                        maxval :max,
                        defaultminval :min,
                        defaultmaxval :max
                    }
                });
            }
			this.drawGraph();
        }
    }
    
    undo() {
        if (this.undoBuffer.length > 1) {
            this.redoBuffer.unshift(this.undoBuffer.shift());
            var undoValue = this.undoBuffer[0];
            this.value = JSON.parse(undoValue);
            this.changeCallback();
            this.setState({selValue: 0});
        }
    }
    
    redo() {
        if (this.redoBuffer.length > 0) {
            var redoValue = this.redoBuffer.shift();
            this.value = JSON.parse(redoValue);
            this.undoBuffer.unshift(redoValue);
            this.changeCallback();
            this.setState({selValue: 0});
        }
    }
    
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
            undoframe : {
                lineHeight: "30px",
                width: "300px",
                height: "28px"
            },
            btnframe: {
                textAlign: "center",
                width: "300px",
                height: "30px",
                display: "flex",
                flexDirection: "row"
            },
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
            undoredoButton: {
                borderRadius: "3px 5px",
                width: "60px",
                height: "22px",
                marginLeft : "3px"
            }
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
                <div ref="undoframe" style={styles.undoframe}>
                    <input ref="undoButton" type="button" value={"Undo"} style={styles.undoredoButton} onClick={this.undo}/>
                    <input ref="redoButton" type="button" value={"Redo"} style={styles.undoredoButton}  onClick={this.redo}/>
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
                        <input ref="minInput" type="number" step={0.00001} value={this.state.valMin} onChange={this.minInputChange} style={styles.minmaxInput}
                                onKeyPress={this.onMinKeyPress.bind(this)}
                                onBlur={this.onMinBlur.bind(this)}
                                onFocus={this.onMinFocus.bind(this)} />
                    </div>
                    <div style={styles.numberArea}>
                        <div ref="maxText" className="KCaption" style={styles.minmaxText}>Max</div>
                        <input ref="maxInput" type="number" step={0.00001} value={this.state.valMax} onChange={this.maxInputChange} style={styles.minmaxInput}
                                onKeyPress={this.onMaxKeyPress.bind(this)}
                                onBlur={this.onMaxBlur.bind(this)}
                                onFocus={this.onMaxFocus.bind(this)} />
                    </div>
                </div>
            </div>
        );
    }
}

module.exports = GradientMap;
