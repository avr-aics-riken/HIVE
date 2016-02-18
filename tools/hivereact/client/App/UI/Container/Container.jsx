import React from "react"
import ReactDOM from "react-dom"

export default class Container extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;

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
            container : {
                backgroundColor: "#333",
                margin : "0px",
                padding : "0px",
                width : "50%",
                height: "100px",
                boxShadow : "0px 0px 3px 0px white inset"
            }
        }
    }

    generator(component, key) {
        if(component && component.ui){
            // var r = React.createFactory(component.ui)({name: component.info.name, key: key});
            var r = React.createFactory(component.ui);
            console.log(r({name: component.info.name, key: key}));
            return r({name: component.info.name, key: key});
            // return r;
            // return React.createFactory(component.ui)({name: component.info.name, key: key});
        }
    }

    render() {
        var styles = this.styles();
        return (
            <div style={styles.container}>
                <span>{this.props.components.map((value, key)=>{return this.generator(value, key);})}</span>
            </div>
        );
    }
}
