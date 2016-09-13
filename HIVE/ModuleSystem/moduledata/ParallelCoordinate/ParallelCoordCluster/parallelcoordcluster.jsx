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
        this.SIZE = 0;
        this.PADDING = 0;

        // function
        this.init = this.init.bind(this);
        this.axisSelectionChange = this.axisSelectionChange.bind(this);
        this.axisSelectionDraw = this.axisSelectionDraw.bind(this);
        this.axisSvgDraw = this.axisSvgDraw.bind(this);
        this.getInputValue = this.getInputValue.bind(this);
        this.setInputValue = this.setInputValue.bind(this);
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
        this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
        this.imageRecieved = this.imageRecieved.bind(this);
        this.selectChanged = this.selectChanged.bind(this);
        this.componentDidUpdate = this.componentDidUpdate.bind(this);

        // axis dragging
        this.axisWrapperSvg        = null;
        this.axisEventDragHorizon  = false;// ドラッグしているかどうかのフラグ
        this.axisEventDragVertical = false;
        this.axisEventDefaultX     = 0;    // イベント登録時の起点 X 座標
        this.axisEventDefaultY     = 0;    // イベント登録時の起点 Y 座標
        this.axisHorizonIndex      = null; // horizon の軸インデックス
        this.axisHorizonSelect     = null; // 選択矩形領域
        this.axisHorizonMinHandle  = null; // 選択矩形の最小値側ハンドル
        this.axisHorizonMaxHandle  = null; // 選択矩形の最大値側ハンドル
        this.axisHorizonListeners  = [];   // horizon イベントリスナ
        this.axisVerticalIndex     = null; // vertical の軸インデックス
        this.axisVerticalSelect    = null; // 選択矩形領域
        this.axisVerticalMinHandle = null; // 選択矩形の最小値側ハンドル
        this.axisVerticalMaxHandle = null; // 選択矩形の最大値側ハンドル
        this.axisVerticalListeners  = [];  // horizon イベントリスナ
        this.axisEventFuncHorizon  = this.axisEventFuncHorizon.bind(this);
        this.axisEventFuncVertical = this.axisEventFuncVertical.bind(this);
        this.axisEventFuncMouseUp  = this.axisEventFuncMouseUp.bind(this);

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

    axisEventFuncHorizon(eve){
        if(!this.axisEventDragHorizon){return;}
        let x = eve.pageX - this.axisEventDefaultX;
        this.axisEventDefaultX = eve.pageX;
        let b = this.axisHorizonSelect.getBBox();
        let l = Math.min(this.SIZE + this.PADDING - b.width, Math.max(this.PADDING, b.x + x));
        this.axisHorizonSelect.setAttribute('d',
            'M '  + (l) + ' ' + this.SIZE +
            ' H ' + (l + b.width) + ' v ' + this.PADDING +
            ' H ' + (l) + ' v ' + (-this.PADDING)
        );
    }
    axisEventFuncVertical(eve){
        if(!this.axisEventDragVertical){return;}
        let y = eve.pageY - this.axisEventDefaultY;
        this.axisEventDefaultY = eve.pageY;
        let b = this.axisVerticalSelect.getBBox();
        let l = Math.min(this.SIZE - b.height, Math.max(0, b.y + y));
        this.axisVerticalSelect.setAttribute('d',
            'M 0 ' + (l) +
            ' h ' + this.PADDING + ' V ' + (l + b.height) +
            ' h ' + (-this.PADDING) + ' V ' + (l)
        );
    }
    axisEventFuncMouseUp(eve){
        let targetAxis = -1;
        let horizon = false;
        let bbox = null;
        let len, min, max;
        if(this.axisEventDragHorizon){
            targetAxis = this.axisHorizonIndex;
            bbox = this.axisHorizonSelect.getBBox();
            horizon = true;
        }
        if(this.axisEventDragVertical){
            targetAxis = this.axisVerticalIndex;
            bbox = this.axisVerticalSelect.getBBox();
        }
        this.axisEventDragHorizon = false;
        this.axisEventDragVertical = false;

        console.log('mouseup!!!!!!!!!!!!!!!!');
        if(targetAxis < 0){return;}

        let axisjson = this.parallel.getAllBrushedRange(
            this.parallel.axisArray[targetAxis], true
        );
        let ax = axisjson[targetAxis];
        if(horizon){
            min = (bbox.x - this.PADDING) / this.SIZE;
            max = (bbox.x + bbox.width - this.PADDING) / this.SIZE;
        }else{
            min = 1.0 - (bbox.y + bbox.height) / this.SIZE;
            max = 1.0 - bbox.y / this.SIZE;
        }
        len = ax.range.max - ax.range.min;
        axisjson[targetAxis].brush.min = ax.range.min + min * len;
        axisjson[targetAxis].brush.max = ax.range.min + max * len;
        this.setInputValue('axisjson', axisjson);
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
        // put input
        // ここのインプットの出力時に、brush情報を更新しているので
        // ここを通るようにすれば、image上でのbrushとリンクさせられるはず
        const varname = this.props.node.varname;
        this.props.action.changeNodeInput({
            varname : varname,
            input : obj
        });

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
        let h = ReactDOM.findDOMNode(this.refs.selectAxisDropdownHorizon);
        let v = ReactDOM.findDOMNode(this.refs.selectAxisDropdownVertical);
        if(h.selectedIndex < 1 || v.selectedIndex < 1){return;}
        if(h.selectedIndex === v.selectedIndex){return;}
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
    // 背景のイメージが更新されるのはドロップダウンリストの変更イベントのときだけ
    // 選択状態はparallel上とimage上でリンクする必要があるのでインプットの更新は行うする必要があり
    // このメソッドの内部で SVG の更新処理メソッドを呼んでる
    axisSelectionDraw(v){
        if(!v || !v.hasOwnProperty('length') || v.length === 0){return;}
        if(!this.layer){
            this.plotlayer = ReactDOM.findDOMNode(this.refs.plotlayer);
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

        cx.clearRect(0, 0, width, height);
        let obj = {
            axisHorizonMin: 0,
            axisHorizonMax: 0,
            axisVerticalMin: 0,
            axisVerticalMax: 0
        };
        if(brushedCount > 1 && this.state.activeHorizon > -1 && this.state.activeVertical > -1){
            let k, l;
            k = this.axisHorizonIndex = this.state.activeHorizon;
            l = this.axisVerticalIndex = this.state.activeVertical;
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
            // draw horizon rect
            cx.beginPath();
            cx.strokeStyle = 'rgba(196, 128, 255, 0.5)';
            cx.lineWidth = 1;
            cx.rect(
                -1,
                top * height,
                width + 1,
                (bottom - top) * height
            );
            cx.stroke();
            cx.closePath();
            // draw vertical rect
            cx.beginPath();
            cx.rect(
                left * width,
                -1,
                (right - left) * width,
                height + 1
            );
            cx.stroke();
            cx.closePath();
            // draw cross rect
            cx.strokeStyle = 'rgba(196, 32, 64, 0.5)';
            cx.lineWidth = 2;
            cx.beginPath();
            cx.rect(
                left * width,
                top * height,
                (right - left) * width,
                (bottom - top) * height
            );
            cx.stroke();
            cx.closePath();
            this.axisSvgDraw([axisArray[k], axisArray[l]]);
        }
        this.setState(obj);
    }

    axisSvgDraw(axisArray){
        let NS_SVG = 'http://www.w3.org/2000/svg';
        let NS = (e)=>{return document.createElementNS(NS_SVG, e);};
        let PADDING = this.PADDING = 20;
        let SIZE = this.SIZE = 256;
        let LINE_COLOR = 'rgba(64, 64, 128, 0.5)';
        let LINE_WIDTH = 2;
        let RECT_FILL_COLOR = 'rgba(196, 196, 196, 0.2)';
        let RECT_STROKE_COLOR = 'rgba(196, 32, 64, 0.5)';

        let wrapperDiv, wrapperSvg;
        let xLen, yLen, left, right, top, bottom;
        let e, f, g, h, i, j, k, l;

        // reset event listener
        for(i = 0, j = this.axisHorizonListeners.length; i < j; ++i){
            this.axisHorizonListeners[i]();
        }
        for(i = 0, j = this.axisVerticalListeners.length; i < j; ++i){
            this.axisVerticalListeners[i]();
        }
        this.axisHorizonListeners = [];
        this.axisVerticalListeners = [];
        // reset svg area
        wrapperDiv = ReactDOM.findDOMNode(this.refs.axisPlotLayer);
        wrapperDiv.innerHTML = '';

        wrapperSvg = NS('svg');
        wrapperSvg.style.display = 'block';
        wrapperSvg.style.width = '100%';
        wrapperSvg.style.height = '100%';
        wrapperSvg.style.margin = '0';
        wrapperSvg.style.padding = '0';
        wrapperDiv.appendChild(wrapperSvg);
        this.axisWrapperSvg = wrapperSvg;
        // line of horizon
        e = NS('path');
        e.setAttribute('stroke', LINE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M ' + PADDING + ' ' + (SIZE + PADDING / 2) + ' h ' + SIZE
        );
        wrapperSvg.appendChild(e);
        e = NS('path');
        e.setAttribute('stroke', LINE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M ' + PADDING + ' ' + SIZE + ' v ' + PADDING
        );
        wrapperSvg.appendChild(e);
        // line of vertical
        e = NS('path');
        e.setAttribute('stroke', LINE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M ' + (PADDING / 2) + ' 0 v ' + SIZE
        );
        wrapperSvg.appendChild(e);
        e = NS('path');
        e.setAttribute('stroke', LINE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M 0 ' + SIZE + ' h ' + PADDING
        );
        wrapperSvg.appendChild(e);

        // coords
        xLen    = axisArray[0].volmax - axisArray[0].volmin;
        left    = (axisArray[0].min - axisArray[0].volmin) / xLen;
        right   = (axisArray[0].max - axisArray[0].volmin) / xLen;
        yLen    = axisArray[1].volmax - axisArray[1].volmin;
        top     = 1.0 - (axisArray[1].max - axisArray[1].volmin) / yLen;
        bottom  = 1.0 - (axisArray[1].min - axisArray[1].volmin) / yLen;

        // selection Rect
        e = NS('path');
        e.setAttribute('fill', RECT_FILL_COLOR);
        e.setAttribute('stroke', RECT_STROKE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M '  + (PADDING + SIZE * left) + ' ' + SIZE +
            ' H ' + (PADDING + SIZE * right) + ' v ' + PADDING +
            ' H ' + (PADDING + SIZE * left) + ' v ' + (-PADDING)
        );
        wrapperSvg.appendChild(e);
        f = ((eve)=>{
            this.axisEventDragHorizon = true;
            this.axisEventDefaultX = eve.pageX;
        }).bind(this);
        this.axisHorizonListeners.push(((func)=>{
            return ()=>{this.axisHorizonSelect.removeEventListener('mousedown', func, false);};
        })(f));
        e.addEventListener('mousedown', f, false);
        this.axisHorizonSelect = e; // add element to prop
        e = NS('path');
        e.setAttribute('fill', RECT_FILL_COLOR);
        e.setAttribute('stroke', RECT_STROKE_COLOR);
        e.setAttribute('stroke-width', LINE_WIDTH);
        e.setAttribute(
            'd',
            'M 0 ' + (SIZE * top) +
            ' h ' + PADDING + ' V ' + (SIZE * bottom) +
            ' h ' + (-PADDING) + ' V ' + (SIZE * top)
        );
        wrapperSvg.appendChild(e);
        f = ((eve)=>{
            this.axisEventDragVertical = true;
            this.axisEventDefaultY = eve.pageY;
        }).bind(this);
        this.axisVerticalListeners.push(((func)=>{
            return ()=>{this.axisVerticalSelect.removeEventListener('mousedown', func, false);};
        })(f));
        e.addEventListener('mousedown', f, false);
        this.axisVerticalSelect = e; // add element to prop

        // draggable rect horizon
        e = NS('path');
        e.setAttribute('fill', 'transparent');
        e.setAttribute('stroke', 'transparent');
        e.setAttribute('style', 'cursor: col-resize');
        e.setAttribute(
            'd',
            'M '  + (PADDING + SIZE * left - 3) + ' ' + SIZE +
            ' H ' + (PADDING + SIZE * left + 3) + ' v ' + PADDING +
            ' H ' + (PADDING + SIZE * left - 3) + ' v ' + (-PADDING)
        );
        wrapperSvg.appendChild(e);
        e = NS('path');
        e.setAttribute('fill', 'transparent');
        e.setAttribute('stroke', 'transparent');
        e.setAttribute('style', 'cursor: col-resize');
        e.setAttribute(
            'd',
            'M '  + (PADDING + SIZE * right - 3) + ' ' + SIZE +
            ' H ' + (PADDING + SIZE * right + 3) + ' v ' + PADDING +
            ' H ' + (PADDING + SIZE * right - 3) + ' v ' + (-PADDING)
        );
        wrapperSvg.appendChild(e);

        // draggable rect vertical
        e = NS('path');
        e.setAttribute('fill', 'transparent');
        e.setAttribute('stroke', 'transparent');
        e.setAttribute('style', 'cursor: row-resize');
        e.setAttribute(
            'd',
            'M 0 '  + (SIZE * top - 3) +
            ' h ' + PADDING + ' V ' + (SIZE * top + 3) +
            ' h ' + (-PADDING) + ' V ' + (SIZE * top - 3)
        );
        wrapperSvg.appendChild(e);
        e = NS('path');
        e.setAttribute('fill', 'transparent');
        e.setAttribute('stroke', 'transparent');
        e.setAttribute('style', 'cursor: row-resize');
        e.setAttribute(
            'd',
            'M 0 '  + (SIZE * bottom - 3) +
            ' h ' + PADDING + ' V ' + (SIZE * bottom + 3) +
            ' h ' + (-PADDING) + ' V ' + (SIZE * bottom - 3)
        );
        wrapperSvg.appendChild(e);

        // last append
        wrapperDiv.appendChild(wrapperSvg);
    }


    nodeInputChanged(){
    }

    componentDidMount(){
        let fdn = ReactDOM.findDOMNode(this.refs.axisPlotLayer);
        fdn.addEventListener('mousemove', this.axisEventFuncHorizon, false);
        fdn.addEventListener('mousemove', this.axisEventFuncVertical, false);
        fdn.addEventListener('mouseup', this.axisEventFuncMouseUp, false);
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

