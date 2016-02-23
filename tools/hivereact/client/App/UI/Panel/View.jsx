import React from "react"
import ReactDOM from "react-dom"
import Container from "./Container.jsx"
import Core from '../../Core'

export default class View extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;

        this.options = props.options;

        this.generator = this.generator.bind(this);

        this.state = {
            components: [].concat(this.store.getComponents())
        };

        // this.componentDidUpdate = this.componentDidUpdate.bind(this);
        // this.componentDidMount = this.componentDidMount.bind(this);

        this.props.store.on(Core.Store.ADD_COMPONENT, function(data){
            this.setState({components: [].concat(this.store.getComponents())});
        }.bind(this));
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
        switch(this.props.options.type){
            default :
                res = React.createFactory(component)({store: this.store, action: this.action, key: key});
            break
        }
        return (
            <Container component={res} key={key} title="test title" />
        );
    }

    render() {
        var styles = this.styles();
        var a = (
            <div className={this.props.options.type} style={styles[this.props.options.type]}>
                {this.state.components.map((value, key)=>{
                    return this.generator(value, key);
                })}
            </div>
        );
        return a;
    }
}
