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

        // function
        this.init = this.init.bind(this);
        this.getInputValue = this.getInputValue.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);

        let source = this.getInputValue('clusterdata');
        if(source === '' || source === '{}' || !source.match(/^(\[|\{)/)){source = `
{"axis": [
    {
        "title": "title_A",
        "cluster": [
            {"top": 0.5, "min": 0.0, "max": 1.0},
            {"top": 2.5, "min": 1.0, "max": 3.0},
            {"top": 4.5, "min": 3.0, "max": 8.0}
        ]
    },{
        "title": "title_B",
        "cluster": [
            {"top": -5.0, "min": -10.0, "max": -5.0},
            {"top": -2.5, "min":  -5.0, "max":  0.0},
            {"top":  1.5, "min":   0.0, "max":  3.0},
            {"top": 11.5, "min":   3.0, "max": 20.0},
            {"top": 45.0, "min":  20.0, "max": 70.0}
        ]
    }
]}`;}
        this.state = {
            clusterdata: JSON.parse(source),
            width: 600,
            height: 300
        };
    }

    // global initialize
    init(json){
        var i, j;
        // this.parallel initialize
        this.parallel = new ParallelCoordCluster(ReactDOM.findDOMNode(this.refs.container));

        if(!json || !json.hasOwnProperty('axis') || json.axis.length < 2){
            console.log('invalid data');
            console.log(json);
            return;
        }

        // axis initialize
        for(i = 0, j = json.axis.length; i < j; ++i){
            this.parallel.addAxis(json.axis[i], i);
        }
        this.parallel.resetAxis();

        // draw canvas
        if(!this.parallel.glReady){return;}
        this.parallel.drawCanvas();
    }

    getInputValue(key){
        for(let i = 0; i < this.node.input.length; ++i){
            if(this.node.input[i].name === key){
                return this.node.input[i].value;
            }
        }
    }

    componentDidMount(){
        // panel change
        this.store.on("panel_size_changed", this.onPanelSizeChanged);

        this.init(this.state.clusterdata);
        // this.init({axis: [
        //     // 軸の配列
        //     {
        //         "title": "title_1",
        //         "cluster": [
        //             // その軸に含まれるクラスタの配列
        //             {"top": 0.5, "min": 0.0, "max": 1.0},
        //             {"top": 2.5, "min": 1.0, "max": 3.0},
        //             {"top": 4.5, "min": 3.0, "max": 8.0}
        //         ]
        //     },{
        //         "title": "title_2",
        //         "cluster": [
        //             {"top": -7.0, "min": -10.0, "max": -5.0},
        //             {"top": -4.5, "min":  -5.0, "max":  0.0},
        //             {"top":  0.5, "min":   0.0, "max":  3.0},
        //             {"top": 10.0, "min":   3.0, "max": 20.0}
        //         ]
        //     },{
        //         "title": "title_3",
        //         "cluster": [
        //             {"top": -5.0, "min": -10.0, "max": -5.0},
        //             {"top": -2.5, "min":  -5.0, "max":  0.0},
        //             {"top":  1.5, "min":   0.0, "max":  3.0},
        //             {"top": 11.5, "min":   3.0, "max": 20.0},
        //             {"top": 45.0, "min":  20.0, "max": 70.0},
        //         ]
        //     }
        // ]});
    }

    componentWillUnmount(){
        this.store.off("panel_size_changed", this.onPanelSizeChanged);
    }

    onPanelSizeChanged(err, data){
        this.setState({
            width: data.panel.size[0],
            height: data.panel.size[1],
        });
        if(!this.parallel.glReady){return;}
        this.parallel.setRect(data.panel.size[0], data.panel.size[1]);
        this.parallel.resetAxis();
    }

    styles(){
        return {
            container: {
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

