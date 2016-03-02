'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _react = require('react');

var _react2 = _interopRequireDefault(_react);

var _reactDom = require('react-dom');

var _reactDom2 = _interopRequireDefault(_reactDom);

var _Pane = require('./Pane');

var _Pane2 = _interopRequireDefault(_Pane);

var _Resizer = require('./Resizer');

var _Resizer2 = _interopRequireDefault(_Resizer);

var _reactVendorPrefix = require('react-vendor-prefix');

var _reactVendorPrefix2 = _interopRequireDefault(_reactVendorPrefix);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = _react2.default.createClass({
    displayName: 'Splitter',
    getInitialState: function getInitialState() {
        return {
            active: false,
            resized: false
        };
    },
    getDefaultProps: function getDefaultProps() {
        return {
            split: 'vertical',
            minSize: 0
        };
    },
    componentDidMount: function componentDidMount() {
        document.addEventListener('mouseup', this.onMouseUp);
        document.addEventListener('mousemove', this.onMouseMove);
        var ref = this.refs.pane1;
        if (ref && this.props.defaultSize !== undefined && !this.state.resized) {
            ref.setState({
                size: this.props.defaultSize
            });
        }
    },
    componentWillUnmount: function componentWillUnmount() {
        document.removeEventListener('mouseup', this.onMouseUp);
        document.removeEventListener('mousemove', this.onMouseMove);
    },
    onMouseDown: function onMouseDown(event) {
        this.unFocus();
        var position = this.props.split === 'vertical' ? event.clientX : event.clientY;
        if (this.props.onDragStart) {
            this.props.onDragStart();
        }
        this.setState({
            active: true,
            position: position
        });
    },
    onMouseMove: function onMouseMove(event) {
        if (this.state.active) {
            this.unFocus();
            var ref = this.refs.pane1;
            if (ref) {
                var node = _reactDom2.default.findDOMNode(ref);
                if (node.getBoundingClientRect) {
                    if(this.props.dontmoved){return;}
                    var width = node.getBoundingClientRect().width;
                    var height = node.getBoundingClientRect().height;
                    var current = this.props.split === 'vertical' ? event.clientX : event.clientY;
                    var size = this.props.split === 'vertical' ? width : height;
                    var position = this.state.position;

                    var newSize = size - (position - current);
                    this.setState({
                        position: current,
                        resized: true
                    });

                    if (newSize < this.props.minSize) {
                        newSize = this.props.minSize;
                    }

                    if (this.props.onChange) {
                        this.props.onChange(newSize);
                    }
                    ref.setState({
                        size: newSize
                    });
                }
            }
        }
    },
    onMouseUp: function onMouseUp() {
        if (this.state.active) {
            if (this.props.onDragFinished) {
                this.props.onDragFinished();
            }
            this.setState({
                active: false
            });
        }
    },
    unFocus: function unFocus() {
        if (document.selection) {
            document.selection.empty();
        } else {
            window.getSelection().removeAllRanges();
        }
    },


    merge: function merge(into, obj) {
        for (var attr in obj) {
            into[attr] = obj[attr];
        }
    },

    render: function render() {

        var split = this.props.split;

        var style = {
            display: 'flex',
            flex: 1,
            position: 'relative',
            outline: 'none',
            overflow: 'hidden',
            MozUserSelect: 'text',
            WebkitUserSelect: 'text',
            msUserSelect: 'text',
            userSelect: 'text'
        };

        if (split === 'vertical') {
            this.merge(style, {
                flexDirection: 'row',
                height: '100%',
                position: 'absolute',
                left: 0,
                right: 0
            });
        } else {
            this.merge(style, {
                flexDirection: 'column',
                height: '100%',
                minHeight: '100%',
                position: 'absolute',
                top: 0,
                bottom: 0,
                width: '100%'
            });
        }

        var children = this.props.children;
        var classes = ['SplitPane', split];
        var prefixed = _reactVendorPrefix2.default.prefix({ styles: style });

        return _react2.default.createElement(
            'div',
            { className: classes.join(' '), style: prefixed.styles, ref: 'splitPane' },
            _react2.default.createElement(
                _Pane2.default,
                { ref: 'pane1', key: 'pane1', split: split },
                children[0]
            ),
            _react2.default.createElement(_Resizer2.default, { ref: 'resizer', key: 'resizer', onMouseDown: this.onMouseDown, split: split }),
            _react2.default.createElement(
                _Pane2.default,
                { ref: 'pane2', key: 'pane2', split: split },
                children[1]
            )
        );
    }
});
module.exports = exports['default'];
