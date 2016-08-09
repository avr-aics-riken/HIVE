import React from 'react';
import ReactDOM from "react-dom";

class ParallelContainer extends React.Component {
    constructor(props){
        super(props);

        // member
        this.store  = this.props.store;
        this.action = this.props.action;
        this.node   = this.props.node;
        this.state = {
            width: 500,
            height: 300
        };

        console.log('cluster!', this.node);

        // variables
        this.parallel = null;

        // function
        this.init = this.init.bind(this);
        this.getInputValue = this.getInputValue.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
    }

    // global initialize
    init(json){
        var i, j;
        if(!json || !json.hasOwnProperty('axis') || json.axis.length < 2){
            console.log('invalid data');
            console.log(json);
            return;
        }

        // this.parallel initialize
        this.parallel = new ParallelCoordCluster(ReactDOM.findDOMNode(this.refs.container));

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

        this.init({"axis": [
            {
                "title": "title1",
                "cluster": [
                    {
                        "min": 0,
                        "max": 5,
                        "out": [0.8, 0.1, 0.1]
                    },{
                        "min": 5,
                        "max": 10,
                        "out": [0.1, 0.8, 0.1]
                    },{
                        "min": 10,
                        "max": 20,
                        "out": [0.1, 0.1, 0.8]
                    },{
                        "min": 20,
                        "max": 30,
                        "out": [0.0, 1.0, 0.0]
                    },{
                        "min": 30,
                        "max": 50,
                        "out": [0.5, 0.0, 0.5]
                    }
                ]
            },{
                "title": "title2",
                "cluster": [
                    {
                        "min": 0,
                        "max": 1,
                        "out": [0.1, 0.5, 0.3, 0.1]
                    },{
                        "min": 1,
                        "max": 2,
                        "out": [0.1, 0.1, 0.5, 0.3]
                    },{
                        "min": 2,
                        "max": 3,
                        "out": [0.3, 0.1, 0.1, 0.5]
                    }
                ]
            },{
                "title": "title3",
                "cluster": [
                    {
                        "min": -5,
                        "max": -2,
                        "out": []
                    },{
                        "min": -2,
                        "max": 2,
                        "out": []
                    },{
                        "min": 2,
                        "max": 5,
                        "out": []
                    },{
                        "min": 5,
                        "max": 10,
                        "out": []
                    }
                ]
            }
        ]});
    }

    componentWillUnmount(){
        this.store.off("panel_size_changed", this.onPanelSizeChanged);
    }

    onPanelSizeChanged(err, data){
        this.setState({
            width: data.panel.size[0],
            height: data.panel.size[1],
        });
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

