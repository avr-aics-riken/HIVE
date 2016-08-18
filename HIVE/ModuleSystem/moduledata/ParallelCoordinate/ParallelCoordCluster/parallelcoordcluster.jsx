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
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.selectChanged = this.selectChanged.bind(this);

        this.state = {
            width: 600,
            height: 300
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

    selectChanged(value, res){
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

        let allMin =  10000.0;
        let allMax = -10000.0;
        for(let j = 0; j < value[0].cluster.length; ++j){
            let minVal = value[0].cluster[j].min;
            let maxVal = value[0].cluster[j].max;
            //console.log('cluster min/max:', minVal, maxVal);
            allMin = Math.min(minVal, allMin);
            allMax = Math.max(maxVal, allMax);
        }
        //console.log('search min/max:', allMin, allMax);

        const numVals = 256;
        let rgba = [];
        rgba.length = numVals*4;
        rgba.fill(0);

        for(let i = 0; i < numVals; ++i){
            let rate = i / numVals;
            for(let j = 0; j < value[0].cluster.length; ++j){
                let selected = value[0].cluster[j].selected;
                if (selected) {
                    let minVal = value[0].cluster[j].min;
                    let maxVal = value[0].cluster[j].max;
                    let cmin = (minVal - allMin) / (allMax - allMin);
                    let cmax = (maxVal - allMin) / (allMax - allMin);
                    if (rate >= cmin && rate <= cmax) {
                        rgba[4*i] = 255; //r
                        rgba[4*i+1] = 255; //g
                        rgba[4*i+2] = 255; //b
                        rgba[4*i+3] = 255; //a
                    }
                }
            }
        }

        const varname = this.props.node.varname;
        res.rgba = rgba;
        this.props.action.changeNodeInput({
            varname: varname,
            input: res
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
            container: {
                backgroundColor: "#333",
                width: this.state.width + "px",
                height: this.state.height + "px"
            },
            canvas: {
                width: "100%",
                height: "100%",
                backgroundColor: "rgba(0, 0, 255, 0.1)"
            }
        };
    }

    render(){
        const styles = this.styles();
        return (
            <div>
                <div ref="container" style={styles.container}>
                </div>
            </div>
        );
    }

}

module.exports = ParallelContainer;

