import React from "react"
import ReactDOM from "react-dom"

export default class PanelContainer extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;

        // this.componentDidUpdate = this.componentDidUpdate.bind(this);
        // this.componentDidMount = this.componentDidMount.bind(this);
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
            panelcontainer : {
                backgroundColor: "#666",
                margin : "0px",
                padding : "0px",
                width : "50px",
                height: "50px",
                display: "inline-block",
                boxShadow : "0px 0px 3px 0px skyblue inset"
            }
        }
    }

    render() {
        var styles = this.styles();
        return (
            <div style={styles.panelcontainer}>
                {this.props.component}
            </div>
        );
    }
}
