import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";


// https://juejin.cn/post/7303797715393183796
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const camera = new THREE.PerspectiveCamera(
    45,
    window.innerWidth / window.innerHeight,
    1,
    500
);
camera.position.set(0, 0, 5);

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x8899ff);

const controls = new OrbitControls(camera, renderer.domElement);
const stats = new Stats();

const one = new THREE.Mesh();
function example1() {
    const vertex = /* GLSL */ `
    varying vec2 vUv;

    void main() {
      vUv = uv;

      // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
      gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
  `;

    const fragment = /* GLSL */ `
  uniform float uTime;
  varying vec2 vUv;

  void main() {

    vec3 color1 = vec3(1.0, 1.0, 0.0); // yellow 黄色
    vec3 color2 = vec3(0.0, 1.0, 1.0); // cyan 青色
    float mixer = step(0.5, fract(vUv.y * 5.0));

    vec3 color = mix(color1, color2, mixer);

    gl_FragColor = vec4(color, 1.0);
  }
  `;

    one.geometry = new THREE.SphereGeometry(0.5);

    one.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
    });

    one.position.set(-2.5, 0, 0);
    scene.add(one);
}

example1();

const two = new THREE.Mesh();
function example2() {
    const vertex = /* GLSL */ `
    varying vec2 vUv;

    void main() {
        vUv = uv;

        // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
    `;

    const fragment = /* GLSL */ `
    varying vec2 vUv;
    uniform float uTime;

    void main() {
      vec3 color1 = vec3(1.0, 1.0, 0.0);
      vec3 color2 = vec3(0.0, 1.0, 1.0);

      float mixer = step(0.25, length(vUv - vec2(0.5)));

      vec3 color = mix(color1, color2, mixer);

      gl_FragColor = vec4(color, 1.0);
    }
    `;

    two.geometry = new THREE.BoxGeometry(1, 1, 1);

    two.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        // wireframe: true,
    });

    two.position.set(-0.5, 0, 0);
    scene.add(two);
}
example2();


const three = new THREE.Mesh();
function example3() {
    const vertex = /* GLSL */ `
    varying vec2 vUv;

    void main() {
        vUv = uv;

        // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
    `;

    const fragment = /* GLSL */ `
    varying vec2 vUv;
    uniform float uTime;

    void main() {
      vec3 color1 = vec3(1.0, 1.0, 0.0);
      vec3 color2 = vec3(0.0, 1.0, 1.0);

      float mixer1 = vUv.x + vUv.y;
      mixer1 = clamp(mixer1, 0.0, 1.0);
      float mixer2 = 2.0 - (vUv.x + vUv.y);
      mixer2 = clamp(mixer2, 0.0, 1.0);

      float mixer = mixer1 * mixer2;

      vec3 color = mix(color1, color2, mixer);

      gl_FragColor = vec4(color, 1.0);
    }
    `;

    three.geometry = new THREE.PlaneGeometry(1, 1, 1);

    three.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        // wireframe: true,
    });

    three.position.set(1, 0, 0);
    scene.add(three);
}
example3();


let time = 0;
function animate() {
    time += 0.05;
    one.material.uniforms.uTime.value = time;
    two.material.uniforms.uTime.value = time;
    three.material.uniforms.uTime.value = time;
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();