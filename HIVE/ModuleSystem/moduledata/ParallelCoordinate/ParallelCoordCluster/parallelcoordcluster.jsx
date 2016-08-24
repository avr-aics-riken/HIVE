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

        // const
        this.PANEL_SIZE_CHANGED = "panel_size_changed";
        this.ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
        this.NODE_INPUT_CHANGED = "node_input_changed";
        this.STORE_IMAGE_RECIEVED = "image_revieved";

        // function
        this.init = this.init.bind(this);
        this.getInputValue = this.getInputValue.bind(this);
        this.setInputValue = this.setInputValue.bind(this);
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
        this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
        this.imageRecieved = this.imageRecieved.bind(this);
        this.selectChanged = this.selectChanged.bind(this);
        this.componentDidUpdate = this.componentDidUpdate.bind(this);

        this.state = {
            width: 700,
            height: 400
        };
    }

    // global initialize
    init(json){
        var i, j;

        // selection test
        // this.selectChanged();

        // data check
        if(!json || !json.hasOwnProperty('axis') || json.axis.length < 2){
            console.log('invalid data');
            console.log(json);
            return;
        }

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
                if(selected){
                    console.log('axis: ' + i + ', cluster: ' + j + ', selected: ' + selected + ' max:' + maxVal + ' min:' + minVal, colorVal);
                }
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
        obj.rgba = rgba;
        const varname = this.props.node.varname;
        this.props.action.changeNodeInput({
            varname : varname,
            input : obj
        });
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
            width: data.panel.size[0],
            height: data.panel.size[1],
        });
        if(!this.parallel || !this.parallel.glReady){return;}
        this.parallel.setRect(data.panel.size[0], data.panel.size[1]);
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
                width: (this.state.width - 256) + "px",
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
                    <img id={this.getCanvasName('img')} style={styles.image} src="" ></img>
                </div>
            </div>
        );
    }

}

module.exports = ParallelContainer;

