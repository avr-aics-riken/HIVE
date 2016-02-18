import React from "react"
import ReactDOM from "react-dom"
import Container from "./Container"

export default class View extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;

        this.options = props.options;

        // this.componentDidUpdate = this.componentDidUpdate.bind(this);
        // this.componentDidMount = this.componentDidMount.bind(this);

        // this.store.on(Core.Store.IMAGE_RECIEVED, (err, param, data) => {
        // 	this.setState({
        // 	});
        // });

    }

    // componentDidUpdate() {
    // }
    //
    // componentDidMount() {
    // }
    //
    // componentWillUnmount() {
    // }
    //
    styles() {
        return {
            normal: {
                backgroundColor: this.props.options.bc,
                margin : "0px",
                padding : "0px",
                minWidth : "300px",
                minHeight: "200px",
                overflow: "hidden",
                boxShadow : "0px 0px 3px 0px white inset"
            },
            scalable: {
                backgroundColor: this.props.options.bc,
                margin : "0px",
                padding : "0px",
                minWidth : "300px",
                minHeight: "200px",
                overflow: "hidden",
                boxShadow : "0px 0px 3px 0px white inset"
            }
        }
    }

    generator(component, key) {
        var res = null;
        if(component && component.ui){
            switch(this.props.options.type){
                default :
                    res = React.createFactory(component.ui)({name: component.info.name, key: key});
                break
            }
            return (
                <Container component={res} key={key} />
            );
        }
    }

    render() {
        var styles = this.styles();
        return (
            <div className={this.props.options.type} style={styles[this.props.options.type]}>
                {this.props.components.map((value, key)=>{
                    return this.generator(value, key);
                })}
            </div>
        );
    }
}
