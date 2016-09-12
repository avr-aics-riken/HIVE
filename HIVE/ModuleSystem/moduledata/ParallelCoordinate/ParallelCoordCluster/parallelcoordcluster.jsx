import React from 'react';
import ReactDOM from "react-dom";

class ParallelContainer extends React.Component {
    constructor(props){
        super(props);

        // member
        this.store  = this.props.store;
        this.action = this.props.action;
        this.node   = this.props.node;

        // variables
        this.parallel = null;
        this.plotlayer = null;
        this.plotctx = null;

        // const
        this.PANEL_SIZE_CHANGED = "panel_size_changed";
        this.ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        this.NODE_INPUT_CHANGED = "node_input_changed";
        this.STORE_IMAGE_RECIEVED = "image_revieved";

        // function
        this.init = this.init.bind(this);
        this.axisSelectionChange = this.axisSelectionChange.bind(this);
        this.axisSelectionDraw = this.axisSelectionDraw.bind(this);
        this.getInputValue = this.getInputValue.bind(this);
        this.setInputValue = this.setInputValue.bind(this);
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
        this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
        this.imageRecieved = this.imageRecieved.bind(this);
        this.selectChanged = this.selectChanged.bind(this);
        this.componentDidUpdate = this.componentDidUpdate.bind(this);

        this.state = {
            width: 700,
            height: 430,
            defaultWidth: 700,
            defaultHeight: 430,
            axisCount: 0,
            axisTitles: [],
            axisHorizonMin: 0,
            axisHorizonMax: 0,
            axisVerticalMin: 0,
            axisVerticalMax: 0,
            activeHorizon: -1,
            activeVertical: -1
        };
    }

    // global initialize
    init(json){
        var i, j;
        var titles = [];

        // selection test
        // this.selectChanged();

        // data check
        if(!json || !json.hasOwnProperty('axis') || json.axis.length < 2){
            console.log('invalid data');
            console.log(json);
            return;
        }
        for(i = 0, j = json.axis.length; i < j; ++i){
            titles.push(json.axis[i].title);
        }
        this.setState({
            axisCount: json.axis.length,
            axisTitles: titles
        });

        // this.parallel initialize
        if(!this.parallel){
            this.parallel = new ParallelCoordCluster(
                ReactDOM.findDOMNode(this.refs.container),
                {callback: {selected: this.setInputValue}}
            );
        }

        // add or reset axis
        this.parallel.resetAxis(json);
    }

    selectChanged(value, obj){
        for(let i = 0; i < value.length; ++i){
            for(let j = 0; j < value[i].cluster.length; ++j){
                let selected = value[i].cluster[j].selected;
                let minVal = value[i].cluster[j].min;
                let maxVal = value[i].cluster[j].max;
                let colorVal = value[i].cluster[j].color;
                // if(selected){
                //     console.log('axis: ' + i + ', cluster: ' + j + ', selected: ' + selected + ' max:' + maxVal + ' min:' + minVal, colorVal);
                // }
            }
        }

        let allMin = [];
        let allMax = [];
        for (let i = 0; i < value.length; ++i) {
            let axMin =  10000.0;
            let axMax = -10000.0;
            for(let j = 0; j < value[i].cluster.length; ++j){
                let minVal = value[i].cluster[j].min;
                let maxVal = value[i].cluster[j].max;
                //console.log('cluster min/max:', minVal, maxVal);
                axMin = Math.min(minVal, axMin);
                axMax = Math.max(maxVal, axMax);
            }
            allMin.push(axMin);
            allMax.push(axMax);
        }

        const numVals = 256;
        let rgba = [];
        const volComp = 3;
        const RGBA = 4;
        rgba.length = numVals * RGBA * volComp * 2;
        rgba.fill(0);

        let ax;
        for(let ax = 0; ax < volComp; ++ax){
            for(let i = 0; i < numVals; ++i){
                let rate = i / numVals;
                let allMinMaxDiff = allMax[ax] - allMin[ax];
                for(let j = 0; j < value[ax].cluster.length; ++j){
                    let minVal = value[ax].cluster[j].min;
                    let maxVal = value[ax].cluster[j].max;
                    let cmin = (minVal - allMin[ax]) / allMinMaxDiff;
                    let cmax = (maxVal - allMin[ax]) / allMinMaxDiff;
                    let selected = value[ax].cluster[j].selected;
                    let colorVal = value[ax].cluster[j].color;
                    if (selected) {
                        if (rate >= cmin && rate <= cmax) {
                            rgba[4*(ax * numVals * 2 + i)  ] = 255*colorVal[0]; //r
                            rgba[4*(ax * numVals * 2 + i)+1] = 255*colorVal[1]; //g
                            rgba[4*(ax * numVals * 2 + i)+2] = 255*colorVal[2]; //b
                            rgba[4*(ax * numVals * 2 + i)+3] = 255; //a
                        }
                    }
                }
            }
            rgba[4*(ax * numVals * 2 + numVals)  ] = allMin[ax]; // min
            rgba[4*(ax * numVals * 2 + numVals)+1] = allMax[ax]; // max
        }
        // rgba
        obj.rgba = rgba;

        // rect draw on image
        this.axisSelectionDraw(value);
    }

    getInputValue(key){
        for(let i = 0; i < this.node.input.length; ++i){
            if(this.node.input[i].name === key){
                return this.node.input[i].value;
            }
        }
    }

    setInputValue(key, value){
        let obj = {};
        obj[key] = JSON.stringify(value).replace(/"/g, '\\\"');
        this.selectChanged(value, obj);
    }

    imageRecieved(err, param, data){
        const varname = this.node.varname;
        if(param.varname !== varname){return;}
        console.log(param);
        if (param.hasOwnProperty("isplot")) {
            console.log("paramparam",param);
            this.imageSize = [parseFloat(param.width), parseFloat(param.height)];
            this.setState({
                image : new Blob([data])
            });
            return;
        }
        if(
            !param.hasOwnProperty('mode') ||
            param.mode !== 'raw' ||
            !param.hasOwnProperty('data') ||
            param.data === null ||
            param.data === undefined
        ){
            console.log('invalid recieaved data');
            return;
        }
        console.log('get recieaved data');
        this.init(param.data);
    }

    axisSelectionChange(eve){
        let h = this.refs.selectAxisDropdownHorizon;
        let v = this.refs.selectAxisDropdownVertical;
        if(h.selectedIndex < 1 || v.selectedIndex < 1){return;}
        let obj = {
            plotX: h.selectedIndex - 1,
            plotY: v.selectedIndex - 1
        };
        // put input
        const varname = this.props.node.varname;
        this.props.action.changeNodeInput({
            varname : varname,
            input : obj
        });
        this.setState({
            activeHorizon: obj.plotX,
            activeVertical: obj.plotY
        });
    }
    // ここはあくまでもコンポーネント上の矩形を描画し直すだけ
    // 背景のイメージが更新されるのはドロップダウンリストの変更イベントのときだけ
    axisSelectionDraw(v){
        if(!v || !v.hasOwnProperty('length') || v.length === 0){return;}
        if(!this.layer){
            this.plotlayer = this.refs.plotlayer;
            this.plotctx = this.plotlayer.getContext('2d');
        }

        let c = this.plotlayer;
        let cx = this.plotctx;
        let width = c.clientWidth;
        let height = c.clientHeight;

        let axisArray = [];
        let brushedCount = 0;
        for(let i = 0, j = v.length; i < j; ++i){
            if(!v[i].selectedAxis && !isNaN(parseFloat(v[i].selectedNumber)) && v[i].selectedNumber > -1){
                axisArray[i] = {
                    selectedIndex: v[i].selectedNumber,
                    title: v[i].title,
                    volmin: v[i].volume.min,
                    volmax: v[i].volume.max,
                    min: v[i].brush.min,
                    max: v[i].brush.max
                };
                if(i === this.state.activeHorizon || i === this.state.activeVertical){
                    brushedCount++;
                }
            }
        }

        cx.strokeStyle = 'crimson';
        cx.lineWidth = 2;
        cx.clearRect(0, 0, width, height);
        let obj = {
            axisHorizonMin: 0,
            axisHorizonMax: 0,
            axisVerticalMin: 0,
            axisVerticalMax: 0
        };
        if(brushedCount > 1 && this.state.activeHorizon > -1 && this.state.activeVertical > -1){
            let k, l;
            k = this.state.activeHorizon;
            l = this.state.activeVertical;
            obj = {
                axisHorizonMin: axisArray[k].min,
                axisHorizonMax: axisArray[k].max,
                axisVerticalMin: axisArray[l].min,
                axisVerticalMax: axisArray[l].max
            };
            // draw rect
            let xLen    = axisArray[k].volmax - axisArray[k].volmin;
            let left    = (axisArray[k].min - axisArray[k].volmin) / xLen;
            let right   = (axisArray[k].max - axisArray[k].volmin) / xLen;
            let yLen    = axisArray[l].volmax - axisArray[l].volmin;
            let top     = 1.0 - (axisArray[l].max - axisArray[l].volmin) / yLen;
            let bottom  = 1.0 - (axisArray[l].min - axisArray[l].volmin) / yLen;
            cx.beginPath();
            cx.rect(
                left * width,
                top * height,
                (right - left) * width,
                (bottom - top) * height
            );
            cx.stroke();
            cx.closePath();
        }
        this.setState(obj);
    }

    nodeInputChanged(){
    }

    componentDidMount(){
        this.store.on(this.PANEL_SIZE_CHANGED, this.onPanelSizeChanged);
        this.store.on(this.ANALYZED_DATA_RECIEVED, this.imageRecieved);
        this.store.on(this.NODE_INPUT_CHANGED, this.nodeInputChanged);
        this.store.on(this.STORE_IMAGE_RECIEVED, this.imageRecieved);
    }

    componentWillUnmount(){
        this.store.off(this.PANEL_SIZE_CHANGED, this.onPanelSizeChanged);
        this.store.off(this.ANALYZED_DATA_RECIEVED, this.imageRecieved);
        this.store.off(this.NODE_INPUT_CHANGED, this.nodeInputChanged);
        this.store.off(this.STORE_IMAGE_RECIEVED, this.imageRecieved);
    }

    componentDidUpdate() {
        if (this.state.image) {
            let imgElem = document.getElementById(this.getCanvasName('img'));
            if (imgElem.src) {
                URL.revokeObjectURL(imgElem.src);
            }
            imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"});
        }
    }

    onPanelSizeChanged(err, data){
        if(data.varname !== this.node.varname){return;}
        this.setState({
            width:  Math.max(this.state.defaultWidth, data.panel.size[0]),
            height: Math.max(this.state.defaultHeight, data.panel.size[1]),
        });
        if(!this.parallel || !this.parallel.glReady){return;}
        this.parallel.setRect(this.state.width, this.state.height);
        this.parallel.resetAxis();
    }

    styles(){
        return {
            wrapper: {
                display: 'flex',
                flexDirection: 'row'
            },
            container: {
                backgroundColor: "#333",
                width: (this.state.width - 276) + "px",
                height: this.state.height + "px"
            },
            canvas: {
                width: "100%",
                height: "100%",
                backgroundColor: "rgba(0, 0, 255, 0.1)"
            },
            image : {
                postion : "relative",
                left : "0px",
                top : "0px",
                width: "256px",
                height: "256px",
                display: (this.imageSize ? "block" : "none")
            },
            layer: {
                position: "relative",
                left : "0px",
                top : "-256px",
                width:   (this.imageSize ? "256px" : "0px"),
                height:  (this.imageSize ? "256px" : "0px"),
                display: (this.imageSize ? "block" : "none")
            },
            row: {
                height: "22px",
                display: "flex",
                flexDirection: "row"
            },
            label: {
                fontSize: "small",
                lineHeight: "22px",
                display: "inline-block",
                width: "70%"
            },
            caption: {
                fontSize: "small",
                lineHeight: "22px",
                display: "inline-block",
                width: "30%"
            },
            title: {
                fontSize: "small",
                fontWeight: "bold",
                lineHeight: "22px",
                display: "inline-block",
                width: "100%"
            },
            box: {
                backgroundColor: "white",
                padding: "0px 0px 20px 20px",
                width: "276px",
                height: "276px"
            },
            axisPlotLayer: {
                position: "relative",
                left : "-20px",
                top : "-512px",
                width:   (this.imageSize ? "276px" : "0px"),
                height:  (this.imageSize ? "276px" : "0px"),
                display: (this.imageSize ? "block" : "none")
            }
        };
    }

    getCanvasName(prefix) {
        return prefix + '-' + this.varname;
    }

    render(){
        const styles = this.styles();
        return (
            <div style={styles.wrapper}>
                <div ref="container" style={styles.container}>
                </div>
                <div>
                    <div style={styles.box}>
                        <img id={this.getCanvasName('img')} style={styles.image} src="" ></img>
                        <canvas ref="plotlayer" style={styles.layer} width="256" height="256"></canvas>
                        <div ref="axisPlotLayer" style={styles.axisPlotLayer}></div>
                    </div>
                    <div>
                        <div style={styles.row}>
                            <div style={styles.title}>
                                <select ref="selectAxisDropdownHorizon" onChange={this.axisSelectionChange}>
                                    <option> --- </option>
                                    {this.state.axisTitles.map((value, index)=>{return (<option key={"dropdownhorizontal" + index}>{value}</option>);})}
                                </select>
                            </div>
                        </div>
                        <div style={styles.row}>
                            <div style={styles.caption}> min:</div>
                            <div style={styles.label}>{this.state.axisHorizonMin}</div>
                        </div>
                        <div style={styles.row}>
                            <div style={styles.caption}> max:</div>
                            <div style={styles.label}>{this.state.axisHorizonMax}</div>
                        </div>
                        <div style={styles.row}>
                            <div style={styles.title}>
                                <select ref="selectAxisDropdownVertical" onChange={this.axisSelectionChange}>
                                    <option> --- </option>
                                    {this.state.axisTitles.map((value, index)=>{return (<option key={"dropdownvertical" + index}>{value}</option>);})}
                                </select>
                            </div>
                        </div>
                        <div style={styles.row}>
                            <div style={styles.caption}> min:</div>
                            <div style={styles.label}>{this.state.axisVerticalMin}</div>
                        </div>
                        <div style={styles.row}>
                            <div style={styles.caption}> max:</div>
                            <div style={styles.label}>{this.state.axisVerticalMax}</div>
                        </div>
                    </div>
                </div>
            </div>
        );
    }

}

module.exports = ParallelContainer;

